/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup CAssertReqAssert
 */

/*
 * Copyright (C) 2026 embedded brains GmbH & Co. KG
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems.h>
#include <setjmp.h>
#include <string.h>
#include <rtems/bspIo.h>
#include <rtems/score/isrlevel.h>

#include "tx-support.h"

#include <rtems/test.h>

/**
 * @defgroup CAssertReqAssert spec:/c/assert/req/assert
 *
 * @ingroup TestsuitesValidationNoClock0
 *
 * @{
 */

typedef enum {
  CAssertReqAssert_Pre_NDEBUG_Defined,
  CAssertReqAssert_Pre_NDEBUG_Undefined,
  CAssertReqAssert_Pre_NDEBUG_NA
} CAssertReqAssert_Pre_NDEBUG;

typedef enum {
  CAssertReqAssert_Pre_AssertFunc_Undefined,
  CAssertReqAssert_Pre_AssertFunc_Null,
  CAssertReqAssert_Pre_AssertFunc_String,
  CAssertReqAssert_Pre_AssertFunc_NA
} CAssertReqAssert_Pre_AssertFunc;

typedef enum {
  CAssertReqAssert_Pre_Expression_True,
  CAssertReqAssert_Pre_Expression_False,
  CAssertReqAssert_Pre_Expression_NA
} CAssertReqAssert_Pre_Expression;

typedef enum {
  CAssertReqAssert_Post_Evaluation_Once,
  CAssertReqAssert_Post_Evaluation_None,
  CAssertReqAssert_Post_Evaluation_NA
} CAssertReqAssert_Post_Evaluation;

typedef enum {
  CAssertReqAssert_Post_Result_Return,
  CAssertReqAssert_Post_Result_Fatal,
  CAssertReqAssert_Post_Result_NA
} CAssertReqAssert_Post_Result;

typedef enum {
  CAssertReqAssert_Post_File_Name,
  CAssertReqAssert_Post_File_NA
} CAssertReqAssert_Post_File;

typedef enum {
  CAssertReqAssert_Post_Line_Number,
  CAssertReqAssert_Post_Line_NA
} CAssertReqAssert_Post_Line;

typedef enum {
  CAssertReqAssert_Post_Function_Name,
  CAssertReqAssert_Post_Function_Null,
  CAssertReqAssert_Post_Function_String,
  CAssertReqAssert_Post_Function_NA
} CAssertReqAssert_Post_Function;

typedef enum {
  CAssertReqAssert_Post_Expression_Text,
  CAssertReqAssert_Post_Expression_NA
} CAssertReqAssert_Post_Expression;

typedef enum {
  CAssertReqAssert_Post_Output_Function,
  CAssertReqAssert_Post_Output_NoFunction,
  CAssertReqAssert_Post_Output_None,
  CAssertReqAssert_Post_Output_NA
} CAssertReqAssert_Post_Output;

typedef struct {
  uint16_t Skip : 1;
  uint16_t Pre_NDEBUG_NA : 1;
  uint16_t Pre_AssertFunc_NA : 1;
  uint16_t Pre_Expression_NA : 1;
  uint16_t Post_Evaluation : 2;
  uint16_t Post_Result : 2;
  uint16_t Post_File : 1;
  uint16_t Post_Line : 1;
  uint16_t Post_Function : 2;
  uint16_t Post_Expression : 1;
  uint16_t Post_Output : 2;
} CAssertReqAssert_Entry;

typedef enum {
  ASSERT_FUNC_UNDEFINED,
  ASSERT_FUNC_NULL,
  ASSERT_FUNC_STRING,
  ASSERT_FUNC_COUNT
} AssertFunc;

/**
 * @brief Test context for spec:/c/assert/req/assert test case.
 */
typedef struct {
  /**
   * @brief This member is true, if NDEBUG shall be defined.
   */
  bool ndebug;

  /**
   * @brief This member specifies the definition of __ASSERT_FUNC.
   */
  AssertFunc assert_func;

  /**
   * @brief This member specifies the value of the expression.
   */
  int value;

  /**
   * @brief This member contains the count of expression evaluations.
   */
  size_t evaluations;

  /**
   * @brief This member contains the line number of the assert() invocation.
   */
  int line;

  /**
   * @brief This member references the name of the function which encloses the
   *   assert() invocation.
   */
  const char *function;

  /**
   * @brief This member is true, if the assert() macro expansion returned.
   */
  bool returned;

  /**
   * @brief This member is true, if a fatal error occurred.
   */
  bool fatal;

  /**
   * @brief This member contains the fatal source.
   */
  rtems_fatal_source source;

  /**
   * @brief This member contains a copy of the object referenced by the fatal
   *   code.
   */
  rtems_assert_context assert_context;

  /**
   * @brief This member contains the characters output by the assert() macro
   *   expansion.
   */
  char output[ 512 ];

  /**
   * @brief This member contains the count of characters output by the assert()
   *   macro expansion.
   */
  size_t output_count;

  /**
   * @brief This member contains the kernel character output handler before the
   *   action.
   */
  BSP_output_char_function_type output_char;

  /**
   * @brief This member provides the context to return from the fatal error.
   */
  jmp_buf before_fatal;

  struct {
    /**
     * @brief This member defines the pre-condition states for the next action.
     */
    size_t pcs[ 3 ];

    /**
     * @brief If this member is true, then the test action loop is executed.
     */
    bool in_action_loop;

    /**
     * @brief This member contains the next transition map index.
     */
    size_t index;

    /**
     * @brief This member contains the current transition map entry.
     */
    CAssertReqAssert_Entry entry;

    /**
     * @brief If this member is true, then the current transition variant
     *   should be skipped.
     */
    bool skip;
  } Map;
} CAssertReqAssert_Context;

static CAssertReqAssert_Context CAssertReqAssert_Instance;

static const char *const CAssertReqAssert_PreDesc_NDEBUG[] =
  { "Defined", "Undefined", "NA" };

static const char *const CAssertReqAssert_PreDesc_AssertFunc[] =
  { "Undefined", "Null", "String", "NA" };

static const char *const CAssertReqAssert_PreDesc_Expression[] =
  { "True", "False", "NA" };

static const char *const *const CAssertReqAssert_PreDesc[] = {
  CAssertReqAssert_PreDesc_NDEBUG,
  CAssertReqAssert_PreDesc_AssertFunc,
  CAssertReqAssert_PreDesc_Expression,
  NULL
};

typedef CAssertReqAssert_Context Context;

static const char assert_func_string[] = "Function";

static int Evaluate( Context *ctx )
{
  ++ctx->evaluations;
  return ctx->value;
}

/*
 * Each inclusion of <assert.h> defines the assert() macro according to the
 * definitions of NDEBUG and __ASSERT_FUNC at this point.
 */

#undef NDEBUG
#undef __ASSERT_FUNC
#include <assert.h>

static void Assert( Context *ctx )
{
  ctx->function = "Assert";
  ctx->line = __LINE__ + 1;
  assert( Evaluate( ctx ) );
}

#undef __ASSERT_FUNC
#define __ASSERT_FUNC NULL
#include <assert.h>

static void AssertFuncNull( Context *ctx )
{
  ctx->line = __LINE__ + 1;
  assert( Evaluate( ctx ) );
}

#undef __ASSERT_FUNC
#define __ASSERT_FUNC assert_func_string
#include <assert.h>

static void AssertFuncString( Context *ctx )
{
  ctx->line = __LINE__ + 1;
  assert( Evaluate( ctx ) );
}

#define NDEBUG
#undef __ASSERT_FUNC
#include <assert.h>

static void AssertNDEBUG( Context *ctx )
{
  (void) ctx;
  assert( Evaluate( ctx ) );
}

#undef __ASSERT_FUNC
#define __ASSERT_FUNC NULL
#include <assert.h>

static void AssertNDEBUGFuncNull( Context *ctx )
{
  (void) ctx;
  assert( Evaluate( ctx ) );
}

#undef __ASSERT_FUNC
#define __ASSERT_FUNC assert_func_string
#include <assert.h>

static void AssertNDEBUGFuncString( Context *ctx )
{
  (void) ctx;
  assert( Evaluate( ctx ) );
}

#undef NDEBUG
#undef __ASSERT_FUNC
#include <assert.h>

static void ( *const asserts[ 2 ][ ASSERT_FUNC_COUNT ] )( Context * ) = {
  { Assert, AssertFuncNull, AssertFuncString },
  { AssertNDEBUG, AssertNDEBUGFuncNull, AssertNDEBUGFuncString }
};

static void OutputChar( char c )
{
  Context *ctx;
  size_t   index;

  ctx = T_fixture_context();
  index = ctx->output_count;

  if ( index < sizeof( ctx->output ) - 1 ) {
    ctx->output[ index ] = c;
    ctx->output[ index + 1 ] = '\0';
  }

  ctx->output_count = index + 1;
}

static void Fatal(
  rtems_fatal_source source,
  rtems_fatal_code   code,
  void              *arg
)
{
  Context *ctx;

  ctx = arg;
  BSP_output_char = ctx->output_char;
  SetFatalHandler( NULL, NULL );
  ctx->fatal = true;
  ctx->source = source;

  if ( source == RTEMS_FATAL_SOURCE_ASSERT ) {
    ctx->assert_context = *(const rtems_assert_context *) code;
  }

  _ISR_Set_level( 0 );
  longjmp( ctx->before_fatal, 1 );
}

static void CheckOutput( const Context *ctx, bool function )
{
  char expected[ sizeof( ctx->output ) ];
  int  n;

  /* The kernel character output translates a NL into a CR and a NL */
  if ( function ) {
    T_assert_not_null( ctx->assert_context.function );
    n = T_snprintf(
      expected,
      sizeof( expected ),
      "assertion \"Evaluate( ctx )\" failed: file \"%s\", line %d, "
      "function: %s\r\n",
      __FILE__,
      ctx->line,
      ctx->assert_context.function
    );
  } else {
    n = T_snprintf(
      expected,
      sizeof( expected ),
      "assertion \"Evaluate( ctx )\" failed: file \"%s\", line %d\r\n",
      __FILE__,
      ctx->line
    );
  }

  T_assert_lt_int( n, (int) sizeof( expected ) );
  T_eq_sz( ctx->output_count, (size_t) n );
  T_eq_str( ctx->output, expected );
}

static void CAssertReqAssert_Pre_NDEBUG_Prepare(
  CAssertReqAssert_Context   *ctx,
  CAssertReqAssert_Pre_NDEBUG state
)
{
  switch ( state ) {
    case CAssertReqAssert_Pre_NDEBUG_Defined: {
      /*
       * Where NDEBUG is defined as a macro name at the point in the source
       * file where assert.h is included.
       */
      ctx->ndebug = true;
      break;
    }

    case CAssertReqAssert_Pre_NDEBUG_Undefined: {
      /*
       * Where NDEBUG is not defined as a macro name at the point in the source
       * file where assert.h is included.
       */
      ctx->ndebug = false;
      break;
    }

    case CAssertReqAssert_Pre_NDEBUG_NA:
      break;
  }
}

static void CAssertReqAssert_Pre_AssertFunc_Prepare(
  CAssertReqAssert_Context       *ctx,
  CAssertReqAssert_Pre_AssertFunc state
)
{
  switch ( state ) {
    case CAssertReqAssert_Pre_AssertFunc_Undefined: {
      /*
       * Where __ASSERT_FUNC is not defined as a macro name at the point in the
       * source file where assert.h is included.
       */
      ctx->assert_func = ASSERT_FUNC_UNDEFINED;
      break;
    }

    case CAssertReqAssert_Pre_AssertFunc_Null: {
      /*
       * Where __ASSERT_FUNC is defined as a null pointer constant at the point
       * in the source file where assert.h is included.
       */
      ctx->assert_func = ASSERT_FUNC_NULL;
      break;
    }

    case CAssertReqAssert_Pre_AssertFunc_String: {
      /*
       * Where __ASSERT_FUNC is defined as an expression which references a
       * string at the point in the source file where assert.h is included.
       */
      ctx->assert_func = ASSERT_FUNC_STRING;
      break;
    }

    case CAssertReqAssert_Pre_AssertFunc_NA:
      break;
  }
}

static void CAssertReqAssert_Pre_Expression_Prepare(
  CAssertReqAssert_Context       *ctx,
  CAssertReqAssert_Pre_Expression state
)
{
  switch ( state ) {
    case CAssertReqAssert_Pre_Expression_True: {
      /*
       * While the `expression` parameter compares unequal to 0.
       */
      ctx->value = 1;
      break;
    }

    case CAssertReqAssert_Pre_Expression_False: {
      /*
       * While the `expression` parameter compares equal to 0.
       */
      ctx->value = 0;
      break;
    }

    case CAssertReqAssert_Pre_Expression_NA:
      break;
  }
}

static void CAssertReqAssert_Post_Evaluation_Check(
  CAssertReqAssert_Context        *ctx,
  CAssertReqAssert_Post_Evaluation state
)
{
  switch ( state ) {
    case CAssertReqAssert_Post_Evaluation_Once: {
      /*
       * The `expression` parameter shall be evaluated exactly once.
       */
      T_eq_sz( ctx->evaluations, 1 );
      break;
    }

    case CAssertReqAssert_Post_Evaluation_None: {
      /*
       * The `expression` parameter shall not be evaluated.
       */
      T_eq_sz( ctx->evaluations, 0 );
      break;
    }

    case CAssertReqAssert_Post_Evaluation_NA:
      break;
  }
}

static void CAssertReqAssert_Post_Result_Check(
  CAssertReqAssert_Context    *ctx,
  CAssertReqAssert_Post_Result state
)
{
  switch ( state ) {
    case CAssertReqAssert_Post_Result_Return: {
      /*
       * The assert() macro expansion shall return to the caller.
       */
      T_true( ctx->returned );
      T_false( ctx->fatal );
      break;
    }

    case CAssertReqAssert_Post_Result_Fatal: {
      /*
       * The assert() macro expansion shall terminate the system with the fatal
       * source RTEMS_FATAL_SOURCE_ASSERT and a fatal code which references an
       * object of type rtems_assert_context.
       */
      T_false( ctx->returned );
      T_true( ctx->fatal );
      T_eq_int( ctx->source, RTEMS_FATAL_SOURCE_ASSERT );
      break;
    }

    case CAssertReqAssert_Post_Result_NA:
      break;
  }
}

static void CAssertReqAssert_Post_File_Check(
  CAssertReqAssert_Context  *ctx,
  CAssertReqAssert_Post_File state
)
{
  switch ( state ) {
    case CAssertReqAssert_Post_File_Name: {
      /*
       * The value of the member file of the object referenced by the fatal
       * code shall reference the presumed name of the source file of the
       * assert() invocation.
       */
      T_assert_not_null( ctx->assert_context.file );
      T_eq_str( ctx->assert_context.file, __FILE__ );
      break;
    }

    case CAssertReqAssert_Post_File_NA:
      break;
  }
}

static void CAssertReqAssert_Post_Line_Check(
  CAssertReqAssert_Context  *ctx,
  CAssertReqAssert_Post_Line state
)
{
  switch ( state ) {
    case CAssertReqAssert_Post_Line_Number: {
      /*
       * The value of the member line of the object referenced by the fatal
       * code shall be the presumed line number of the assert() invocation.
       */
      T_eq_int( ctx->assert_context.line, ctx->line );
      break;
    }

    case CAssertReqAssert_Post_Line_NA:
      break;
  }
}

static void CAssertReqAssert_Post_Function_Check(
  CAssertReqAssert_Context      *ctx,
  CAssertReqAssert_Post_Function state
)
{
  switch ( state ) {
    case CAssertReqAssert_Post_Function_Name: {
      /*
       * The value of the member function of the object referenced by the fatal
       * code shall reference the name of the function which encloses the
       * assert() invocation.
       */
      T_assert_not_null( ctx->assert_context.function );
      T_eq_str( ctx->assert_context.function, ctx->function );
      break;
    }

    case CAssertReqAssert_Post_Function_Null: {
      /*
       * The value of the member function of the object referenced by the fatal
       * code shall be NULL.
       */
      T_null( ctx->assert_context.function );
      break;
    }

    case CAssertReqAssert_Post_Function_String: {
      /*
       * The value of the member function of the object referenced by the fatal
       * code shall be the value of __ASSERT_FUNC.
       */
      T_eq_ptr( ctx->assert_context.function, assert_func_string );
      break;
    }

    case CAssertReqAssert_Post_Function_NA:
      break;
  }
}

static void CAssertReqAssert_Post_Expression_Check(
  CAssertReqAssert_Context        *ctx,
  CAssertReqAssert_Post_Expression state
)
{
  switch ( state ) {
    case CAssertReqAssert_Post_Expression_Text: {
      /*
       * The value of the member failed_expression of the object referenced by
       * the fatal code shall reference the text of the `expression` parameter.
       */
      T_assert_not_null( ctx->assert_context.failed_expression );
      T_eq_str( ctx->assert_context.failed_expression, "Evaluate( ctx )" );
      break;
    }

    case CAssertReqAssert_Post_Expression_NA:
      break;
  }
}

static void CAssertReqAssert_Post_Output_Check(
  CAssertReqAssert_Context    *ctx,
  CAssertReqAssert_Post_Output state
)
{
  switch ( state ) {
    case CAssertReqAssert_Post_Output_Function: {
      /*
       * Before the system terminates, the assert() macro expansion shall
       * output with printk() the characters `assertion "`, the text of the
       * `expression` parameter, the characters `" failed: file "`, the
       * presumed name of the source file of the assert() invocation, the
       * characters `", line `, the presumed line number of the assert()
       * invocation in decimal notation, the characters `, function: `, the
       * string referenced by the member function of the object referenced by
       * the fatal code, and a `NL` character in this order.
       */
      CheckOutput( ctx, true );
      break;
    }

    case CAssertReqAssert_Post_Output_NoFunction: {
      /*
       * Before the system terminates, the assert() macro expansion shall
       * output with printk() the characters `assertion "`, the text of the
       * `expression` parameter, the characters `" failed: file "`, the
       * presumed name of the source file of the assert() invocation, the
       * characters `", line `, the presumed line number of the assert()
       * invocation in decimal notation, and a `NL` character in this order.
       */
      CheckOutput( ctx, false );
      break;
    }

    case CAssertReqAssert_Post_Output_None: {
      /*
       * The assert() macro expansion shall output no character.
       */
      T_eq_sz( ctx->output_count, 0 );
      break;
    }

    case CAssertReqAssert_Post_Output_NA:
      break;
  }
}

static void CAssertReqAssert_Action( CAssertReqAssert_Context *ctx )
{
  ctx->evaluations = 0;
  ctx->returned = false;
  ctx->fatal = false;
  ctx->source = 0;
  ctx->output_count = 0;
  ctx->output[ 0 ] = '\0';
  memset( &ctx->assert_context, 0, sizeof( ctx->assert_context ) );
  SetFatalHandler( Fatal, ctx );
  ctx->output_char = BSP_output_char;
  BSP_output_char = OutputChar;

  if ( setjmp( ctx->before_fatal ) == 0 ) {
    ( *asserts[ ctx->ndebug ][ ctx->assert_func ] )( ctx );
    BSP_output_char = ctx->output_char;
    SetFatalHandler( NULL, NULL );
    ctx->returned = true;
  }
}

/* clang-format off */

static const CAssertReqAssert_Entry
CAssertReqAssert_Entries[] = {
  { 0, 0, 0, 0, CAssertReqAssert_Post_Evaluation_None,
    CAssertReqAssert_Post_Result_Return, CAssertReqAssert_Post_File_NA,
    CAssertReqAssert_Post_Line_NA, CAssertReqAssert_Post_Function_NA,
    CAssertReqAssert_Post_Expression_NA, CAssertReqAssert_Post_Output_None },
  { 0, 0, 0, 0, CAssertReqAssert_Post_Evaluation_Once,
    CAssertReqAssert_Post_Result_Return, CAssertReqAssert_Post_File_NA,
    CAssertReqAssert_Post_Line_NA, CAssertReqAssert_Post_Function_NA,
    CAssertReqAssert_Post_Expression_NA, CAssertReqAssert_Post_Output_None },
  { 0, 0, 0, 0, CAssertReqAssert_Post_Evaluation_Once,
    CAssertReqAssert_Post_Result_Fatal, CAssertReqAssert_Post_File_Name,
    CAssertReqAssert_Post_Line_Number, CAssertReqAssert_Post_Function_Name,
    CAssertReqAssert_Post_Expression_Text,
    CAssertReqAssert_Post_Output_Function },
  { 0, 0, 0, 0, CAssertReqAssert_Post_Evaluation_Once,
    CAssertReqAssert_Post_Result_Fatal, CAssertReqAssert_Post_File_Name,
    CAssertReqAssert_Post_Line_Number, CAssertReqAssert_Post_Function_Null,
    CAssertReqAssert_Post_Expression_Text,
    CAssertReqAssert_Post_Output_NoFunction },
  { 0, 0, 0, 0, CAssertReqAssert_Post_Evaluation_Once,
    CAssertReqAssert_Post_Result_Fatal, CAssertReqAssert_Post_File_Name,
    CAssertReqAssert_Post_Line_Number, CAssertReqAssert_Post_Function_String,
    CAssertReqAssert_Post_Expression_Text,
    CAssertReqAssert_Post_Output_Function }
};

static const uint8_t
CAssertReqAssert_Map[] = {
  0, 0, 0, 0, 0, 0, 1, 2, 1, 3, 1, 4
};

/* clang-format on */

static size_t CAssertReqAssert_Scope( void *arg, char *buf, size_t n )
{
  CAssertReqAssert_Context *ctx;

  ctx = arg;

  if ( ctx->Map.in_action_loop ) {
    return T_get_scope( CAssertReqAssert_PreDesc, buf, n, ctx->Map.pcs );
  }

  return 0;
}

static T_fixture CAssertReqAssert_Fixture = {
  .setup = NULL,
  .stop = NULL,
  .teardown = NULL,
  .scope = CAssertReqAssert_Scope,
  .initial_context = &CAssertReqAssert_Instance
};

static inline CAssertReqAssert_Entry CAssertReqAssert_PopEntry(
  CAssertReqAssert_Context *ctx
)
{
  size_t index;

  index = ctx->Map.index;
  ctx->Map.index = index + 1;
  return CAssertReqAssert_Entries[ CAssertReqAssert_Map[ index ] ];
}

static void CAssertReqAssert_TestVariant( CAssertReqAssert_Context *ctx )
{
  CAssertReqAssert_Pre_NDEBUG_Prepare( ctx, ctx->Map.pcs[ 0 ] );
  CAssertReqAssert_Pre_AssertFunc_Prepare( ctx, ctx->Map.pcs[ 1 ] );
  CAssertReqAssert_Pre_Expression_Prepare( ctx, ctx->Map.pcs[ 2 ] );
  CAssertReqAssert_Action( ctx );
  CAssertReqAssert_Post_Evaluation_Check(
    ctx,
    ctx->Map.entry.Post_Evaluation
  );
  CAssertReqAssert_Post_Result_Check( ctx, ctx->Map.entry.Post_Result );
  CAssertReqAssert_Post_File_Check( ctx, ctx->Map.entry.Post_File );
  CAssertReqAssert_Post_Line_Check( ctx, ctx->Map.entry.Post_Line );
  CAssertReqAssert_Post_Function_Check( ctx, ctx->Map.entry.Post_Function );
  CAssertReqAssert_Post_Expression_Check(
    ctx,
    ctx->Map.entry.Post_Expression
  );
  CAssertReqAssert_Post_Output_Check( ctx, ctx->Map.entry.Post_Output );
}

/**
 * @fn void T_case_body_CAssertReqAssert( void )
 */
T_TEST_CASE_FIXTURE( CAssertReqAssert, &CAssertReqAssert_Fixture )
{
  CAssertReqAssert_Context *ctx;

  ctx = T_fixture_context();
  ctx->Map.in_action_loop = true;
  ctx->Map.index = 0;

  for (
    ctx->Map.pcs[ 0 ] = CAssertReqAssert_Pre_NDEBUG_Defined;
    ctx->Map.pcs[ 0 ] < CAssertReqAssert_Pre_NDEBUG_NA;
    ++ctx->Map.pcs[ 0 ]
  ) {
    for (
      ctx->Map.pcs[ 1 ] = CAssertReqAssert_Pre_AssertFunc_Undefined;
      ctx->Map.pcs[ 1 ] < CAssertReqAssert_Pre_AssertFunc_NA;
      ++ctx->Map.pcs[ 1 ]
    ) {
      for (
        ctx->Map.pcs[ 2 ] = CAssertReqAssert_Pre_Expression_True;
        ctx->Map.pcs[ 2 ] < CAssertReqAssert_Pre_Expression_NA;
        ++ctx->Map.pcs[ 2 ]
      ) {
        ctx->Map.entry = CAssertReqAssert_PopEntry( ctx );
        CAssertReqAssert_TestVariant( ctx );
      }
    }
  }
}

/** @} */
