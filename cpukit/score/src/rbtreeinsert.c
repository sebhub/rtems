/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RTEMSScoreRBTree
 *
 * @brief This source file contains the implementation of
 *   _RBTree_Insert_color().
 */

/*
 * Copyright (C) 2026 embedded brains GmbH & Co. KG
 * Copyright (C) 2010-2012 Gedare Bloom.
 * Copyright (C) 2002 Niels Provos <provos@citi.umich.edu>
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

/*
 * The red-black tree code of this file derives from the macros of the
 * FreeBSD <sys/tree.h> file of 2020.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/score/rbtreeimpl.h>

/*
 * The parent of a node lies in the same word as its color.  Where the color
 * of a node is known, a store writes its parent and its color at once.
 */

/*
 * The node and its parent are red.  The function lies apart from
 * _RBTree_Insert_color(), so that the cases without a rebalance save no
 * register.
 */
static RTEMS_NO_INLINE void _RBTree_Insert_rebalance(
  RBTree_Control *head,
  RBTree_Node    *elm,
  RBTree_Node    *parent
)
{
  while ( true ) {
    RBTree_Node *gparent;
    RBTree_Node *uncle;
    RBTree_Node *tmp;

    if ( parent == NULL ) {
      _RBTree_Set_parent_and_color( elm, NULL, RTEMS_RB_BLACK );
      return;
    }

    if ( _RBTree_Color( parent ) == RTEMS_RB_BLACK ) {
      return;
    }

    /* A red parent is not the root, and its parent is black. */
    gparent = _RBTree_Parent_of_color( parent, RTEMS_RB_RED );
    uncle = _RBTree_Right( gparent );

    if ( parent != uncle ) {
      if ( _RBTree_Is_red( uncle ) ) {
        _RBTree_Set_parent_and_color( uncle, gparent, RTEMS_RB_BLACK );
        _RBTree_Set_parent_and_color( parent, gparent, RTEMS_RB_BLACK );
        elm = gparent;
        parent = _RBTree_Parent_of_color( elm, RTEMS_RB_BLACK );
        _RBTree_Set_parent_and_color( elm, parent, RTEMS_RB_RED );
        continue;
      }

      if ( _RBTree_Right( parent ) == elm ) {
        /* Rotate left at the parent.  A child of a red node is black. */
        tmp = _RBTree_Left( elm );
        _RBTree_Set_right( parent, tmp );

        if ( tmp != NULL ) {
          _RBTree_Set_parent_and_color( tmp, parent, RTEMS_RB_BLACK );
        }

        _RBTree_Set_left( elm, parent );
        _RBTree_Set_parent_and_color( parent, elm, RTEMS_RB_RED );
        parent = elm;
      }

      /* Rotate right at the grandparent. */
      tmp = _RBTree_Right( parent );
      _RBTree_Set_left( gparent, tmp );

      if ( tmp != NULL ) {
        _RBTree_Set_parent_and_color( tmp, gparent, RTEMS_RB_BLACK );
      }

      _RBTree_Set_right( parent, gparent );
    } else {
      uncle = _RBTree_Left( gparent );

      if ( _RBTree_Is_red( uncle ) ) {
        _RBTree_Set_parent_and_color( uncle, gparent, RTEMS_RB_BLACK );
        _RBTree_Set_parent_and_color( parent, gparent, RTEMS_RB_BLACK );
        elm = gparent;
        parent = _RBTree_Parent_of_color( elm, RTEMS_RB_BLACK );
        _RBTree_Set_parent_and_color( elm, parent, RTEMS_RB_RED );
        continue;
      }

      if ( _RBTree_Left( parent ) == elm ) {
        /* Rotate right at the parent.  A child of a red node is black. */
        tmp = _RBTree_Right( elm );
        _RBTree_Set_left( parent, tmp );

        if ( tmp != NULL ) {
          _RBTree_Set_parent_and_color( tmp, parent, RTEMS_RB_BLACK );
        }

        _RBTree_Set_right( elm, parent );
        _RBTree_Set_parent_and_color( parent, elm, RTEMS_RB_RED );
        parent = elm;
      }

      /* Rotate left at the grandparent. */
      tmp = _RBTree_Left( parent );
      _RBTree_Set_right( gparent, tmp );

      if ( tmp != NULL ) {
        _RBTree_Set_parent_and_color( tmp, gparent, RTEMS_RB_BLACK );
      }

      _RBTree_Set_left( parent, gparent );
    }

    /* The parent takes the place of the grandparent and becomes black. */
    tmp = _RBTree_Parent_of_color( gparent, RTEMS_RB_BLACK );
    _RBTree_Set_parent_and_color( parent, tmp, RTEMS_RB_BLACK );
    _RBTree_Set_parent_and_color( gparent, parent, RTEMS_RB_RED );
    _RBTree_Relink_child( head, gparent, parent, tmp );
    return;
  }
}

void _RBTree_Insert_color( RBTree_Control *head, RBTree_Node *elm )
{
  RBTree_Node *parent;

  /* The node is red. */
  parent = _RBTree_Parent_of_color( elm, RTEMS_RB_RED );

  if ( parent == NULL ) {
    _RBTree_Set_parent_and_color( elm, NULL, RTEMS_RB_BLACK );
    return;
  }

  if ( _RBTree_Color( parent ) == RTEMS_RB_BLACK ) {
    return;
  }

  _RBTree_Insert_rebalance( head, elm, parent );
}
