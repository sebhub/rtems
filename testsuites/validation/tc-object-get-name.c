/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RtemsObjectReqGetName
 */

/*
 * Copyright (C) 2025 embedded brains GmbH & Co. KG
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
#include <string.h>
#include <rtems/score/objectimpl.h>

#include "ts-config.h"
#include "tx-support.h"

#include <rtems/test.h>

/**
 * @defgroup RtemsObjectReqGetName spec:/rtems/object/req/get-name
 *
 * @ingroup TestsuitesValidationNoClock0
 *
 * @{
 */

typedef enum {
  RtemsObjectReqGetName_Pre_API_Zero,
  RtemsObjectReqGetName_Pre_API_Valid,
  RtemsObjectReqGetName_Pre_API_Invalid,
  RtemsObjectReqGetName_Pre_API_NA
} RtemsObjectReqGetName_Pre_API;

typedef enum {
  RtemsObjectReqGetName_Pre_Class_Zero,
  RtemsObjectReqGetName_Pre_Class_ValidNoObjects,
  RtemsObjectReqGetName_Pre_Class_ValidMaybeObjects,
  RtemsObjectReqGetName_Pre_Class_Invalid,
  RtemsObjectReqGetName_Pre_Class_NA
} RtemsObjectReqGetName_Pre_Class;

typedef enum {
  RtemsObjectReqGetName_Pre_NameType_Integer,
  RtemsObjectReqGetName_Pre_NameType_String,
  RtemsObjectReqGetName_Pre_NameType_NA
} RtemsObjectReqGetName_Pre_NameType;

typedef enum {
  RtemsObjectReqGetName_Pre_Node_Zero,
  RtemsObjectReqGetName_Pre_Node_Valid,
  RtemsObjectReqGetName_Pre_Node_Invalid,
  RtemsObjectReqGetName_Pre_Node_NA
} RtemsObjectReqGetName_Pre_Node;

typedef enum {
  RtemsObjectReqGetName_Pre_Index_Zero,
  RtemsObjectReqGetName_Pre_Index_Valid,
  RtemsObjectReqGetName_Pre_Index_Invalid,
  RtemsObjectReqGetName_Pre_Index_NA
} RtemsObjectReqGetName_Pre_Index;

typedef enum {
  RtemsObjectReqGetName_Pre_NameProperties_Printable,
  RtemsObjectReqGetName_Pre_NameProperties_NonPrintable,
  RtemsObjectReqGetName_Pre_NameProperties_Null,
  RtemsObjectReqGetName_Pre_NameProperties_NA
} RtemsObjectReqGetName_Pre_NameProperties;

typedef enum {
  RtemsObjectReqGetName_Pre_LengthName_ZeroNull,
  RtemsObjectReqGetName_Pre_LengthName_ZeroValid,
  RtemsObjectReqGetName_Pre_LengthName_PositiveNull,
  RtemsObjectReqGetName_Pre_LengthName_Valid,
  RtemsObjectReqGetName_Pre_LengthName_NA
} RtemsObjectReqGetName_Pre_LengthName;

typedef enum {
  RtemsObjectReqGetName_Pre_ObjectNameLength_Fits,
  RtemsObjectReqGetName_Pre_ObjectNameLength_TooLong,
  RtemsObjectReqGetName_Pre_ObjectNameLength_NA
} RtemsObjectReqGetName_Pre_ObjectNameLength;

typedef enum {
  RtemsObjectReqGetName_Post_Result_Null,
  RtemsObjectReqGetName_Post_Result_Buffer,
  RtemsObjectReqGetName_Post_Result_NA
} RtemsObjectReqGetName_Post_Result;

typedef enum {
  RtemsObjectReqGetName_Post_Length_Full,
  RtemsObjectReqGetName_Post_Length_Truncated,
  RtemsObjectReqGetName_Post_Length_Empty,
  RtemsObjectReqGetName_Post_Length_NA
} RtemsObjectReqGetName_Post_Length;

typedef enum {
  RtemsObjectReqGetName_Post_Characters_Copied,
  RtemsObjectReqGetName_Post_Characters_Replaced,
  RtemsObjectReqGetName_Post_Characters_NA
} RtemsObjectReqGetName_Post_Characters;

typedef struct {
  uint16_t Skip : 1;
  uint16_t Pre_API_NA : 1;
  uint16_t Pre_Class_NA : 1;
  uint16_t Pre_NameType_NA : 1;
  uint16_t Pre_Node_NA : 1;
  uint16_t Pre_Index_NA : 1;
  uint16_t Pre_NameProperties_NA : 1;
  uint16_t Pre_LengthName_NA : 1;
  uint16_t Pre_ObjectNameLength_NA : 1;
  uint16_t Post_Result : 2;
  uint16_t Post_Length : 2;
  uint16_t Post_Characters : 2;
} RtemsObjectReqGetName_Entry;

/**
 * @brief Test context for spec:/rtems/object/req/get-name test case.
 */
typedef struct {
  /**
   * @brief This member provides an object information to manipulate.
   */
  Objects_Information *information;

  /**
   * @brief This member contains the original object maximum identifier.
   */
  Objects_Id maximum_id;

  /**
   * @brief This member provides an object to manipulate.
   */
  Objects_Control *object;

  /**
   * @brief This member provides a buffer for the object name.
   */
  char name_buf[ 5 ];
  ;

  /**
   * @brief This member provides the object name.
   */
  char object_name[ 5 ];
  ;

  /**
   * @brief This member provides the `id` parameter value.
   */
  rtems_id id;

  /**
   * @brief This member provides the `length` parameter value.
   */
  size_t length;

  /**
   * @brief This member provides the `name` parameter value.
   */
  char *name;

  /**
   * @brief This member contains the returned result.
   */
  const char *result;

  struct {
    /**
     * @brief This member defines the pre-condition indices for the next
     *   action.
     */
    size_t pci[ 8 ];

    /**
     * @brief This member defines the pre-condition states for the next action.
     */
    size_t pcs[ 8 ];

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
    RtemsObjectReqGetName_Entry entry;

    /**
     * @brief If this member is true, then the current transition variant
     *   should be skipped.
     */
    bool skip;
  } Map;
} RtemsObjectReqGetName_Context;

static RtemsObjectReqGetName_Context RtemsObjectReqGetName_Instance;

static const char *const RtemsObjectReqGetName_PreDesc_API[] =
  { "Zero", "Valid", "Invalid", "NA" };

static const char *const RtemsObjectReqGetName_PreDesc_Class[] =
  { "Zero", "ValidNoObjects", "ValidMaybeObjects", "Invalid", "NA" };

static const char *const RtemsObjectReqGetName_PreDesc_NameType[] =
  { "Integer", "String", "NA" };

static const char *const RtemsObjectReqGetName_PreDesc_Node[] =
  { "Zero", "Valid", "Invalid", "NA" };

static const char *const RtemsObjectReqGetName_PreDesc_Index[] =
  { "Zero", "Valid", "Invalid", "NA" };

static const char *const RtemsObjectReqGetName_PreDesc_NameProperties[] =
  { "Printable", "NonPrintable", "Null", "NA" };

static const char *const RtemsObjectReqGetName_PreDesc_LengthName[] =
  { "ZeroNull", "ZeroValid", "PositiveNull", "Valid", "NA" };

static const char *const RtemsObjectReqGetName_PreDesc_ObjectNameLength[] =
  { "Fits", "TooLong", "NA" };

static const char *const *const RtemsObjectReqGetName_PreDesc[] = {
  RtemsObjectReqGetName_PreDesc_API,
  RtemsObjectReqGetName_PreDesc_Class,
  RtemsObjectReqGetName_PreDesc_NameType,
  RtemsObjectReqGetName_PreDesc_Node,
  RtemsObjectReqGetName_PreDesc_Index,
  RtemsObjectReqGetName_PreDesc_NameProperties,
  RtemsObjectReqGetName_PreDesc_LengthName,
  RtemsObjectReqGetName_PreDesc_ObjectNameLength,
  NULL
};

static const char printable_name[] = "1234";

static const char non_printable_name[] = "\x01\x1f\x7f\xff";

static const char converted_name[] = "****";

static void RtemsObjectReqGetName_Pre_API_Prepare(
  RtemsObjectReqGetName_Context *ctx,
  RtemsObjectReqGetName_Pre_API  state
)
{
  switch ( state ) {
    case RtemsObjectReqGetName_Pre_API_Zero: {
      /*
       * While the API of the `id` parameter is zero.
       */
      /* Already prepared */
      break;
    }

    case RtemsObjectReqGetName_Pre_API_Valid: {
      /*
       * While the API of the `id` parameter is associated with an object API.
       */
      ctx->id |= OBJECTS_CLASSIC_API << OBJECTS_API_START_BIT;
      break;
    }

    case RtemsObjectReqGetName_Pre_API_Invalid: {
      /*
       * While the API of the `id` parameter is not equal to zero and is
       * associated with no object API.
       */
      ctx->id |= OBJECTS_APIS_LAST << OBJECTS_API_START_BIT;
      break;
    }

    case RtemsObjectReqGetName_Pre_API_NA:
      break;
  }
}

static void RtemsObjectReqGetName_Pre_Class_Prepare(
  RtemsObjectReqGetName_Context  *ctx,
  RtemsObjectReqGetName_Pre_Class state
)
{
  switch ( state ) {
    case RtemsObjectReqGetName_Pre_Class_Zero: {
      /*
       * While the class of the `id` parameter is zero.
       */
      /* Already prepared */
      break;
    }

    case RtemsObjectReqGetName_Pre_Class_ValidNoObjects: {
      /*
       * While the class of the `id` parameter is associated with an object
       * class which has no object configured.
       */
      ctx->id |= OBJECTS_RTEMS_TASKS << OBJECTS_CLASS_START_BIT;
      ctx->information->maximum_id = 0;
      break;
    }

    case RtemsObjectReqGetName_Pre_Class_ValidMaybeObjects: {
      /*
       * While the class of the `id` parameter is associated with an object
       * class which has at least one object configured.
       */
      ctx->id |= OBJECTS_RTEMS_TASKS << OBJECTS_CLASS_START_BIT;
      ctx->information->maximum_id = ctx->maximum_id;
      break;
    }

    case RtemsObjectReqGetName_Pre_Class_Invalid: {
      /*
       * While the class of the `id` parameter is not equal to zero and is
       * associated with no object class.
       */
      ctx->id |= OBJECTS_RTEMS_CLASSES_LAST << OBJECTS_CLASS_START_BIT;

      /* Use this to increase the coverage of _Objects_API_maximum_class */
      if ( rtems_object_id_get_api( ctx->id ) == OBJECTS_CLASSIC_API ) {
        ctx->id &= ~OBJECTS_API_MASK;
        ctx->id |= OBJECTS_INTERNAL_API << OBJECTS_API_START_BIT;
      }
      break;
    }

    case RtemsObjectReqGetName_Pre_Class_NA:
      break;
  }
}

static void RtemsObjectReqGetName_Pre_NameType_Prepare(
  RtemsObjectReqGetName_Context     *ctx,
  RtemsObjectReqGetName_Pre_NameType state
)
{
  switch ( state ) {
    case RtemsObjectReqGetName_Pre_NameType_Integer: {
      /*
       * While the object class uses integer names.
       */
      ctx->information->name_length = 0;
      break;
    }

    case RtemsObjectReqGetName_Pre_NameType_String: {
      /*
       * While the object class uses string names.
       */
      ctx->information->name_length = RTEMS_ARRAY_SIZE( ctx->name_buf );
      break;
    }

    case RtemsObjectReqGetName_Pre_NameType_NA:
      break;
  }
}

static void RtemsObjectReqGetName_Pre_Node_Prepare(
  RtemsObjectReqGetName_Context *ctx,
  RtemsObjectReqGetName_Pre_Node state
)
{
  switch ( state ) {
    case RtemsObjectReqGetName_Pre_Node_Zero: {
      /*
       * While the node of the `id` parameter is zero.
       */
      /* Already prepared */
      break;
    }

    case RtemsObjectReqGetName_Pre_Node_Valid: {
      /*
       * While the node of the `id` parameter is associated with a node.
       */
      ctx->id |= 1 << OBJECTS_NODE_START_BIT;
      break;
    }

    case RtemsObjectReqGetName_Pre_Node_Invalid: {
      /*
       * While the node of the `id` parameter is not equal to zero and is
       * associated with no node.
       */
      ctx->id |= 2 << OBJECTS_NODE_START_BIT;
      break;
    }

    case RtemsObjectReqGetName_Pre_Node_NA:
      break;
  }
}

static void RtemsObjectReqGetName_Pre_Index_Prepare(
  RtemsObjectReqGetName_Context  *ctx,
  RtemsObjectReqGetName_Pre_Index state
)
{
  switch ( state ) {
    case RtemsObjectReqGetName_Pre_Index_Zero: {
      /*
       * While the index of the `id` parameter is zero.
       */
      /* Already prepared */
      break;
    }

    case RtemsObjectReqGetName_Pre_Index_Valid: {
      /*
       * While the index of the `id` parameter is associated with an object.
       */
      ctx->id |= 1 << OBJECTS_INDEX_START_BIT;
      break;
    }

    case RtemsObjectReqGetName_Pre_Index_Invalid: {
      /*
       * While the index of the `id` parameter is not equal to zero and is
       * associated with no object.
       */
      ctx->id |= 2 << OBJECTS_INDEX_START_BIT;
      break;
    }

    case RtemsObjectReqGetName_Pre_Index_NA:
      break;
  }
}

static void RtemsObjectReqGetName_Pre_NameProperties_Prepare(
  RtemsObjectReqGetName_Context           *ctx,
  RtemsObjectReqGetName_Pre_NameProperties state
)
{
  const char *name;

  name = ctx->object_name;

  switch ( state ) {
    case RtemsObjectReqGetName_Pre_NameProperties_Printable: {
      /*
       * While all characters of the object name are printable.
       */
      memcpy( ctx->object_name, printable_name, sizeof( ctx->object_name ) );
      break;
    }

    case RtemsObjectReqGetName_Pre_NameProperties_NonPrintable: {
      /*
       * While at least one character of the object name is not printable.
       */
      memcpy(
        ctx->object_name,
        non_printable_name,
        sizeof( ctx->object_name )
      );
      break;
    }

    case RtemsObjectReqGetName_Pre_NameProperties_Null: {
      /*
       * While the string name pointer of the object is NULL.
       */
      name = NULL;
      memset( ctx->object_name, 0, sizeof( ctx->object_name ) );
      break;
    }

    case RtemsObjectReqGetName_Pre_NameProperties_NA:
      break;
  }

  if ( _Objects_Has_string_name( ctx->information ) ) {
    ctx->object->name.name_p = name;
  } else if ( name == NULL ) {
    ctx->object->name.name_u32 = 0;
  } else {
    ctx->object->name.name_u32 = rtems_build_name(
      name[ 0 ],
      name[ 1 ],
      name[ 2 ],
      name[ 3 ]
    );
  }
}

static void RtemsObjectReqGetName_Pre_LengthName_Prepare(
  RtemsObjectReqGetName_Context       *ctx,
  RtemsObjectReqGetName_Pre_LengthName state
)
{
  switch ( state ) {
    case RtemsObjectReqGetName_Pre_LengthName_ZeroNull: {
      /*
       * While the `length` parameter is zero and the `name` parameter is NULL.
       */
      ctx->length = 0;
      ctx->name = NULL;
      break;
    }

    case RtemsObjectReqGetName_Pre_LengthName_ZeroValid: {
      /*
       * While the `length` parameter is zero and the `name` parameter
       * references a buffer.
       */
      ctx->length = 0;
      ctx->name = ctx->name_buf;
      break;
    }

    case RtemsObjectReqGetName_Pre_LengthName_PositiveNull: {
      /*
       * While the `length` parameter is greater than zero and the `name`
       * parameter is NULL.
       */
      ctx->length = 1;
      ctx->name = NULL;
      break;
    }

    case RtemsObjectReqGetName_Pre_LengthName_Valid: {
      /*
       * While the `length` parameter is greater than zero and the `name`
       * parameter references a buffer of at least this size.
       */
      ctx->length = RTEMS_ARRAY_SIZE( ctx->name_buf );
      ctx->name = ctx->name_buf;
      break;
    }

    case RtemsObjectReqGetName_Pre_LengthName_NA:
      break;
  }
}

static void RtemsObjectReqGetName_Pre_ObjectNameLength_Prepare(
  RtemsObjectReqGetName_Context             *ctx,
  RtemsObjectReqGetName_Pre_ObjectNameLength state
)
{
  switch ( state ) {
    case RtemsObjectReqGetName_Pre_ObjectNameLength_Fits: {
      /*
       * While the count of characters of the object name is less than the
       * value of the `length` parameter.
       */
      /* Already prepared */
      break;
    }

    case RtemsObjectReqGetName_Pre_ObjectNameLength_TooLong: {
      /*
       * While the count of characters of the object name is greater than or
       * equal to the value of the `length` parameter.
       */
      ctx->length = 2;
      break;
    }

    case RtemsObjectReqGetName_Pre_ObjectNameLength_NA:
      break;
  }
}

static void RtemsObjectReqGetName_Post_Result_Check(
  RtemsObjectReqGetName_Context    *ctx,
  RtemsObjectReqGetName_Post_Result state
)
{
  switch ( state ) {
    case RtemsObjectReqGetName_Post_Result_Null: {
      /*
       * The return value of rtems_object_get_name() shall be NULL.
       */
      T_null( ctx->result );
      break;
    }

    case RtemsObjectReqGetName_Post_Result_Buffer: {
      /*
       * The return value of rtems_object_get_name() shall be the value of the
       * `name` parameter.
       */
      T_eq_ptr( ctx->result, ctx->name_buf );
      break;
    }

    case RtemsObjectReqGetName_Post_Result_NA:
      break;
  }
}

static void RtemsObjectReqGetName_Post_Length_Check(
  RtemsObjectReqGetName_Context    *ctx,
  RtemsObjectReqGetName_Post_Length state
)
{
  switch ( state ) {
    case RtemsObjectReqGetName_Post_Length_Full: {
      /*
       * The count of characters of the string referenced by the `name`
       * parameter shall be the count of characters of the object name.
       */
      T_eq_sz( strlen( ctx->name_buf ), strlen( ctx->object_name ) );
      break;
    }

    case RtemsObjectReqGetName_Post_Length_Truncated: {
      /*
       * The count of characters of the string referenced by the `name`
       * parameter shall be the value of the `length` parameter minus one.
       */
      T_eq_sz( strlen( ctx->name_buf ), ctx->length - 1 );
      break;
    }

    case RtemsObjectReqGetName_Post_Length_Empty: {
      /*
       * The string referenced by the `name` parameter shall be the empty
       * string.
       */
      T_eq_sz( strlen( ctx->name_buf ), 0 );
      break;
    }

    case RtemsObjectReqGetName_Post_Length_NA:
      break;
  }
}

static void RtemsObjectReqGetName_Post_Characters_Check(
  RtemsObjectReqGetName_Context        *ctx,
  RtemsObjectReqGetName_Post_Characters state
)
{
  switch ( state ) {
    case RtemsObjectReqGetName_Post_Characters_Copied: {
      /*
       * Each character of the string referenced by the `name` parameter shall
       * be the character of the object name at the same position.
       */
      T_eq_nstr( ctx->name_buf, ctx->object_name, strlen( ctx->name_buf ) );
      break;
    }

    case RtemsObjectReqGetName_Post_Characters_Replaced: {
      /*
       * Each character of the string referenced by the `name` parameter at the
       * position of a non-printable character of the object name shall be an
       * asterisk.
       */
      T_eq_nstr( ctx->name_buf, converted_name, strlen( ctx->name_buf ) );
      break;
    }

    case RtemsObjectReqGetName_Post_Characters_NA:
      break;
  }
}

static void RtemsObjectReqGetName_Setup( RtemsObjectReqGetName_Context *ctx )
{
  ctx->information = _Objects_Information_table[ OBJECTS_CLASSIC_API ]
                                               [ OBJECTS_RTEMS_TASKS ];
  ctx->maximum_id = ctx->information->maximum_id;
  ctx->object = &GetExecuting()->Object;
}

static void RtemsObjectReqGetName_Setup_Wrap( void *arg )
{
  RtemsObjectReqGetName_Context *ctx;

  ctx = arg;
  ctx->Map.in_action_loop = false;
  RtemsObjectReqGetName_Setup( ctx );
}

static void RtemsObjectReqGetName_Teardown(
  RtemsObjectReqGetName_Context *ctx
)
{
  ctx->information->maximum_id = ctx->maximum_id;
  ctx->information->name_length = 0;
  ctx->object->name.name_u32 = TEST_RUNNER_NAME;
}

static void RtemsObjectReqGetName_Teardown_Wrap( void *arg )
{
  RtemsObjectReqGetName_Context *ctx;

  ctx = arg;
  ctx->Map.in_action_loop = false;
  RtemsObjectReqGetName_Teardown( ctx );
}

static void RtemsObjectReqGetName_Prepare( RtemsObjectReqGetName_Context *ctx )
{
  ctx->id = 0;
  memset( ctx->name_buf, 0xff, sizeof( ctx->name_buf ) );
}

static void RtemsObjectReqGetName_Action( RtemsObjectReqGetName_Context *ctx )
{
  ctx->result = rtems_object_get_name( ctx->id, ctx->length, ctx->name );
}

/* clang-format off */

static const RtemsObjectReqGetName_Entry
RtemsObjectReqGetName_Entries[] = {
  { 1, 0, 0, 0, 0, 0, 0, 0, 0, RtemsObjectReqGetName_Post_Result_NA,
    RtemsObjectReqGetName_Post_Length_NA,
    RtemsObjectReqGetName_Post_Characters_NA },
  { 1, 0, 0, 1, 0, 0, 1, 0, 1, RtemsObjectReqGetName_Post_Result_Null,
    RtemsObjectReqGetName_Post_Length_NA,
    RtemsObjectReqGetName_Post_Characters_NA },
  { 1, 0, 0, 1, 0, 0, 1, 0, 1, RtemsObjectReqGetName_Post_Result_Null,
    RtemsObjectReqGetName_Post_Length_NA,
    RtemsObjectReqGetName_Post_Characters_NA },
  { 1, 0, 0, 0, 0, 0, 1, 0, 1, RtemsObjectReqGetName_Post_Result_Null,
    RtemsObjectReqGetName_Post_Length_NA,
    RtemsObjectReqGetName_Post_Characters_NA },
  { 0, 0, 0, 1, 0, 0, 1, 0, 1, RtemsObjectReqGetName_Post_Result_Null,
    RtemsObjectReqGetName_Post_Length_NA,
    RtemsObjectReqGetName_Post_Characters_NA },
  { 0, 0, 0, 0, 0, 0, 1, 0, 1, RtemsObjectReqGetName_Post_Result_Null,
    RtemsObjectReqGetName_Post_Length_NA,
    RtemsObjectReqGetName_Post_Characters_NA },
  { 1, 0, 0, 0, 0, 0, 0, 0, 0, RtemsObjectReqGetName_Post_Result_Null,
    RtemsObjectReqGetName_Post_Length_NA,
    RtemsObjectReqGetName_Post_Characters_NA },
  { 1, 0, 0, 0, 0, 0, 0, 0, 1, RtemsObjectReqGetName_Post_Result_Null,
    RtemsObjectReqGetName_Post_Length_NA,
    RtemsObjectReqGetName_Post_Characters_NA },
  { 1, 0, 0, 0, 0, 0, 0, 0, 1, RtemsObjectReqGetName_Post_Result_Null,
    RtemsObjectReqGetName_Post_Length_NA,
    RtemsObjectReqGetName_Post_Characters_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 1, RtemsObjectReqGetName_Post_Result_Null,
    RtemsObjectReqGetName_Post_Length_NA,
    RtemsObjectReqGetName_Post_Characters_NA },
  { 0, 0, 0, 1, 0, 0, 0, 0, 1, RtemsObjectReqGetName_Post_Result_Null,
    RtemsObjectReqGetName_Post_Length_NA,
    RtemsObjectReqGetName_Post_Characters_NA },
  { 0, 0, 0, 0, 0, 1, 1, 0, 1, RtemsObjectReqGetName_Post_Result_Null,
    RtemsObjectReqGetName_Post_Length_NA,
    RtemsObjectReqGetName_Post_Characters_NA },
  { 1, 0, 0, 0, 0, 1, 0, 0, 1, RtemsObjectReqGetName_Post_Result_Null,
    RtemsObjectReqGetName_Post_Length_NA,
    RtemsObjectReqGetName_Post_Characters_NA },
  { 1, 0, 0, 0, 0, 1, 0, 0, 0, RtemsObjectReqGetName_Post_Result_Null,
    RtemsObjectReqGetName_Post_Length_NA,
    RtemsObjectReqGetName_Post_Characters_NA },
  { 1, 0, 0, 1, 0, 0, 0, 0, 0, RtemsObjectReqGetName_Post_Result_Null,
    RtemsObjectReqGetName_Post_Length_NA,
    RtemsObjectReqGetName_Post_Characters_NA },
  { 1, 0, 0, 0, 0, 0, 0, 0, 0, RtemsObjectReqGetName_Post_Result_Buffer,
    RtemsObjectReqGetName_Post_Length_Empty,
    RtemsObjectReqGetName_Post_Characters_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, RtemsObjectReqGetName_Post_Result_Buffer,
    RtemsObjectReqGetName_Post_Length_Full,
    RtemsObjectReqGetName_Post_Characters_Copied },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, RtemsObjectReqGetName_Post_Result_Buffer,
    RtemsObjectReqGetName_Post_Length_Truncated,
    RtemsObjectReqGetName_Post_Characters_Copied },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, RtemsObjectReqGetName_Post_Result_Buffer,
    RtemsObjectReqGetName_Post_Length_Full,
    RtemsObjectReqGetName_Post_Characters_Replaced },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, RtemsObjectReqGetName_Post_Result_Buffer,
    RtemsObjectReqGetName_Post_Length_Truncated,
    RtemsObjectReqGetName_Post_Characters_Replaced },
  { 0, 0, 0, 1, 0, 0, 0, 0, 0, RtemsObjectReqGetName_Post_Result_Buffer,
    RtemsObjectReqGetName_Post_Length_Full,
    RtemsObjectReqGetName_Post_Characters_Copied },
  { 0, 0, 0, 1, 0, 0, 0, 0, 0, RtemsObjectReqGetName_Post_Result_Buffer,
    RtemsObjectReqGetName_Post_Length_Truncated,
    RtemsObjectReqGetName_Post_Characters_Copied },
  { 0, 0, 0, 1, 0, 0, 0, 0, 0, RtemsObjectReqGetName_Post_Result_Buffer,
    RtemsObjectReqGetName_Post_Length_Full,
    RtemsObjectReqGetName_Post_Characters_Replaced },
  { 0, 0, 0, 1, 0, 0, 0, 0, 0, RtemsObjectReqGetName_Post_Result_Buffer,
    RtemsObjectReqGetName_Post_Length_Truncated,
    RtemsObjectReqGetName_Post_Characters_Replaced },
  { 0, 0, 0, 1, 0, 0, 0, 0, 0, RtemsObjectReqGetName_Post_Result_Buffer,
    RtemsObjectReqGetName_Post_Length_Empty,
    RtemsObjectReqGetName_Post_Characters_NA },
  { 1, 0, 0, 1, 0, 0, 0, 0, 0, RtemsObjectReqGetName_Post_Result_Buffer,
    RtemsObjectReqGetName_Post_Length_Empty,
    RtemsObjectReqGetName_Post_Characters_NA },
  { 1, 0, 0, 0, 0, 0, 0, 0, 0, RtemsObjectReqGetName_Post_Result_Buffer,
    RtemsObjectReqGetName_Post_Length_Full,
    RtemsObjectReqGetName_Post_Characters_Copied },
  { 1, 0, 0, 0, 0, 0, 0, 0, 0, RtemsObjectReqGetName_Post_Result_Buffer,
    RtemsObjectReqGetName_Post_Length_Truncated,
    RtemsObjectReqGetName_Post_Characters_Copied },
  { 1, 0, 0, 0, 0, 0, 0, 0, 0, RtemsObjectReqGetName_Post_Result_Buffer,
    RtemsObjectReqGetName_Post_Length_Full,
    RtemsObjectReqGetName_Post_Characters_Replaced },
  { 1, 0, 0, 0, 0, 0, 0, 0, 0, RtemsObjectReqGetName_Post_Result_Buffer,
    RtemsObjectReqGetName_Post_Length_Truncated,
    RtemsObjectReqGetName_Post_Characters_Replaced },
  { 1, 0, 0, 0, 0, 0, 0, 0, 0, RtemsObjectReqGetName_Post_Result_Buffer,
    RtemsObjectReqGetName_Post_Length_Empty,
    RtemsObjectReqGetName_Post_Characters_NA },
  { 1, 0, 0, 0, 0, 0, 0, 0, 0, RtemsObjectReqGetName_Post_Result_Buffer,
    RtemsObjectReqGetName_Post_Length_Empty,
    RtemsObjectReqGetName_Post_Characters_NA },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, RtemsObjectReqGetName_Post_Result_Buffer,
    RtemsObjectReqGetName_Post_Length_Empty,
    RtemsObjectReqGetName_Post_Characters_NA }
};

static const uint8_t
RtemsObjectReqGetName_Map[] = {
  10, 0, 10, 0, 10, 0, 20, 21, 10, 0, 10, 0, 10, 0, 22, 23, 10, 14, 10, 14, 10,
  14, 24, 25, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 4, 0, 4, 0, 4, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  4, 0, 4, 0, 4, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 0, 4, 0,
  4, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 4, 0, 4, 0,
  4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 0, 4, 0, 4, 0, 4, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 8, 8, 8, 8, 8, 26, 27, 8, 8, 8,
  8, 8, 8, 28, 29, 8, 6, 8, 6, 8, 6, 30, 15, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 4, 0, 4, 0, 4, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 4, 0, 4, 0, 4, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4,
  0, 4, 0, 4, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 0, 4, 0, 4,
  0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 4, 0, 4, 0, 4,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 0, 4, 0, 4, 0, 4, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 4, 0, 4,
  0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 0, 4, 0, 4, 0, 4, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 4, 0, 4, 0, 4, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 0, 4, 0, 4, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 4, 0, 4, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 4, 0, 4, 0, 4, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 5, 0, 5, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 7, 6, 7, 6, 7, 6, 7, 6, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 7,
  6, 7, 6, 7, 6, 7, 6, 5, 0, 5, 0, 5, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 6, 7,
  6, 7, 6, 7, 6, 5, 0, 5, 0, 5, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 6, 7, 6, 7,
  6, 7, 6, 11, 0, 11, 0, 11, 0, 11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 12, 13, 12, 13,
  12, 13, 12, 13, 5, 0, 5, 0, 5, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 6, 7, 6,
  7, 6, 7, 6, 5, 0, 5, 0, 5, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 6, 7, 6, 7, 6,
  7, 6, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 7, 6, 7, 6, 7, 6, 7, 6,
  5, 0, 5, 0, 5, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 6, 7, 6, 7, 6, 7, 6, 5, 0,
  5, 0, 5, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3,
  3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 5, 0, 5, 0, 5, 0,
  5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 5, 0, 5, 0, 5, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11, 0, 11, 0, 11, 0, 11, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 5, 0, 5, 0, 5, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 5, 0, 5, 0, 5, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
  3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 5, 0, 5, 0, 5, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 5, 0, 5, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 6,
  7, 6, 7, 6, 7, 6, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 7, 6, 7, 6,
  7, 6, 7, 6, 5, 0, 5, 0, 5, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 6, 7, 6, 7, 6,
  7, 6, 5, 0, 5, 0, 5, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 6, 7, 6, 7, 6, 7, 6,
  9, 0, 9, 0, 9, 0, 16, 17, 9, 0, 9, 0, 9, 0, 18, 19, 7, 6, 7, 6, 7, 6, 31, 15,
  5, 0, 5, 0, 5, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 6, 7, 6, 7, 6, 7, 6, 5, 0,
  5, 0, 5, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 6, 7, 6, 7, 6, 7, 6, 3, 3, 3, 3,
  3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 7, 6, 7, 6, 7, 6, 7, 6, 5, 0, 5, 0, 5, 0,
  5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 6, 7, 6, 7, 6, 7, 6, 5, 0, 5, 0, 5, 0, 5, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
  3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 5, 0, 5, 0, 5, 0, 5, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 5, 0, 5, 0, 5, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 0, 9, 0, 9, 0, 16, 17, 9, 0, 9, 0, 9, 0, 18,
  19, 9, 6, 9, 6, 9, 6, 32, 15, 5, 0, 5, 0, 5, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 5, 0, 5, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
  3, 3, 3, 3, 5, 0, 5, 0, 5, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 4, 0, 4, 0, 4, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 0,
  4, 0, 4, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 4, 0,
  4, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 0, 4, 0, 4, 0, 4, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 4, 0, 4, 0, 4, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 0, 4, 0, 4, 0, 4, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 4, 0, 4, 0, 4, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 0, 4, 0, 4, 0, 4, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 4, 0, 4, 0, 4, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 4, 0, 4, 0, 4, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 4, 0, 4, 0, 4, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 4, 0, 4, 0, 4, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4, 0, 4, 0, 4, 0, 4, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 0, 4, 0, 4, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 4, 0, 4, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 4, 0, 4, 0, 4, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 4, 0, 4, 0, 4, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 0,
  4, 0, 4, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0
};

/* clang-format on */

static size_t RtemsObjectReqGetName_Scope( void *arg, char *buf, size_t n )
{
  RtemsObjectReqGetName_Context *ctx;

  ctx = arg;

  if ( ctx->Map.in_action_loop ) {
    return T_get_scope( RtemsObjectReqGetName_PreDesc, buf, n, ctx->Map.pcs );
  }

  return 0;
}

static T_fixture RtemsObjectReqGetName_Fixture = {
  .setup = RtemsObjectReqGetName_Setup_Wrap,
  .stop = NULL,
  .teardown = RtemsObjectReqGetName_Teardown_Wrap,
  .scope = RtemsObjectReqGetName_Scope,
  .initial_context = &RtemsObjectReqGetName_Instance
};

static inline RtemsObjectReqGetName_Entry RtemsObjectReqGetName_PopEntry(
  RtemsObjectReqGetName_Context *ctx
)
{
  size_t index;

  index = ctx->Map.index;
  ctx->Map.index = index + 1;
  return RtemsObjectReqGetName_Entries[ RtemsObjectReqGetName_Map[ index ] ];
}

static void RtemsObjectReqGetName_SetPreConditionStates(
  RtemsObjectReqGetName_Context *ctx
)
{
  ctx->Map.pcs[ 0 ] = ctx->Map.pci[ 0 ];
  ctx->Map.pcs[ 1 ] = ctx->Map.pci[ 1 ];

  if ( ctx->Map.entry.Pre_NameType_NA ) {
    ctx->Map.pcs[ 2 ] = RtemsObjectReqGetName_Pre_NameType_NA;
  } else {
    ctx->Map.pcs[ 2 ] = ctx->Map.pci[ 2 ];
  }

  ctx->Map.pcs[ 3 ] = ctx->Map.pci[ 3 ];

  if ( ctx->Map.entry.Pre_Index_NA ) {
    ctx->Map.pcs[ 4 ] = RtemsObjectReqGetName_Pre_Index_NA;
  } else {
    ctx->Map.pcs[ 4 ] = ctx->Map.pci[ 4 ];
  }

  if ( ctx->Map.entry.Pre_NameProperties_NA ) {
    ctx->Map.pcs[ 5 ] = RtemsObjectReqGetName_Pre_NameProperties_NA;
  } else {
    ctx->Map.pcs[ 5 ] = ctx->Map.pci[ 5 ];
  }

  ctx->Map.pcs[ 6 ] = ctx->Map.pci[ 6 ];

  if ( ctx->Map.entry.Pre_ObjectNameLength_NA ) {
    ctx->Map.pcs[ 7 ] = RtemsObjectReqGetName_Pre_ObjectNameLength_NA;
  } else {
    ctx->Map.pcs[ 7 ] = ctx->Map.pci[ 7 ];
  }
}

static void RtemsObjectReqGetName_TestVariant(
  RtemsObjectReqGetName_Context *ctx
)
{
  RtemsObjectReqGetName_Pre_API_Prepare( ctx, ctx->Map.pcs[ 0 ] );
  RtemsObjectReqGetName_Pre_Class_Prepare( ctx, ctx->Map.pcs[ 1 ] );
  RtemsObjectReqGetName_Pre_NameType_Prepare( ctx, ctx->Map.pcs[ 2 ] );
  RtemsObjectReqGetName_Pre_Node_Prepare( ctx, ctx->Map.pcs[ 3 ] );
  RtemsObjectReqGetName_Pre_Index_Prepare( ctx, ctx->Map.pcs[ 4 ] );
  RtemsObjectReqGetName_Pre_NameProperties_Prepare( ctx, ctx->Map.pcs[ 5 ] );
  RtemsObjectReqGetName_Pre_LengthName_Prepare( ctx, ctx->Map.pcs[ 6 ] );
  RtemsObjectReqGetName_Pre_ObjectNameLength_Prepare( ctx, ctx->Map.pcs[ 7 ] );
  RtemsObjectReqGetName_Action( ctx );
  RtemsObjectReqGetName_Post_Result_Check( ctx, ctx->Map.entry.Post_Result );
  RtemsObjectReqGetName_Post_Length_Check( ctx, ctx->Map.entry.Post_Length );
  RtemsObjectReqGetName_Post_Characters_Check(
    ctx,
    ctx->Map.entry.Post_Characters
  );
}

/**
 * @fn void T_case_body_RtemsObjectReqGetName( void )
 */
T_TEST_CASE_FIXTURE( RtemsObjectReqGetName, &RtemsObjectReqGetName_Fixture )
{
  RtemsObjectReqGetName_Context *ctx;

  ctx = T_fixture_context();
  ctx->Map.in_action_loop = true;
  ctx->Map.index = 0;

  for (
    ctx->Map.pci[ 0 ] = RtemsObjectReqGetName_Pre_API_Zero;
    ctx->Map.pci[ 0 ] < RtemsObjectReqGetName_Pre_API_NA;
    ++ctx->Map.pci[ 0 ]
  ) {
    for (
      ctx->Map.pci[ 1 ] = RtemsObjectReqGetName_Pre_Class_Zero;
      ctx->Map.pci[ 1 ] < RtemsObjectReqGetName_Pre_Class_NA;
      ++ctx->Map.pci[ 1 ]
    ) {
      for (
        ctx->Map.pci[ 2 ] = RtemsObjectReqGetName_Pre_NameType_Integer;
        ctx->Map.pci[ 2 ] < RtemsObjectReqGetName_Pre_NameType_NA;
        ++ctx->Map.pci[ 2 ]
      ) {
        for (
          ctx->Map.pci[ 3 ] = RtemsObjectReqGetName_Pre_Node_Zero;
          ctx->Map.pci[ 3 ] < RtemsObjectReqGetName_Pre_Node_NA;
          ++ctx->Map.pci[ 3 ]
        ) {
          for (
            ctx->Map.pci[ 4 ] = RtemsObjectReqGetName_Pre_Index_Zero;
            ctx->Map.pci[ 4 ] < RtemsObjectReqGetName_Pre_Index_NA;
            ++ctx->Map.pci[ 4 ]
          ) {
            for (
              ctx->Map.pci[ 5 ] =
                RtemsObjectReqGetName_Pre_NameProperties_Printable;
              ctx->Map.pci[ 5 ] < RtemsObjectReqGetName_Pre_NameProperties_NA;
              ++ctx->Map.pci[ 5 ]
            ) {
              for (
                ctx->Map.pci[ 6 ] =
                  RtemsObjectReqGetName_Pre_LengthName_ZeroNull;
                ctx->Map.pci[ 6 ] < RtemsObjectReqGetName_Pre_LengthName_NA;
                ++ctx->Map.pci[ 6 ]
              ) {
                for (
                  ctx->Map.pci[ 7 ] =
                    RtemsObjectReqGetName_Pre_ObjectNameLength_Fits;
                  ctx->Map.pci[ 7 ] <
                  RtemsObjectReqGetName_Pre_ObjectNameLength_NA;
                  ++ctx->Map.pci[ 7 ]
                ) {
                  ctx->Map.entry = RtemsObjectReqGetName_PopEntry( ctx );

                  if ( ctx->Map.entry.Skip ) {
                    continue;
                  }

                  RtemsObjectReqGetName_SetPreConditionStates( ctx );
                  RtemsObjectReqGetName_Prepare( ctx );
                  RtemsObjectReqGetName_TestVariant( ctx );
                }
              }
            }
          }
        }
      }
    }
  }
}

/** @} */
