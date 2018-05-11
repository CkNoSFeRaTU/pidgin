/*
 * purple - Jabber Protocol Plugin
 *
 * Purple is the legal property of its developers, whose names are too numerous
 * to list here.  Please refer to the COPYRIGHT file distributed with this
 * source distribution.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02111-1301  USA
 *
 */

#ifndef PURPLE_JABBER_MAM_H_
#define PURPLE_JABBER_MAM_H_

#include "xmlnode.h"

enum {
    MAM_PREFS_NEVER = 0,
    MAM_PREFS_ROSTER,
    MAM_PREFS_ALWAYS
};

typedef struct {
    char *start;
    char *end;
    char *with;
    gboolean completed;
} mam_item_t;

typedef struct {
    GList *queue;
    mam_item_t *current;
    char last_timestamp[64];
    guint32 count;
    char *ns;
} mam_t;

void jabber_mam_clear(mam_t *mam);
void jabber_mam_add_to_queue(JabberStream *js, const char* start, const char* end, const char* with);
void jabber_mam_process(JabberStream *js, const char* after);
void jabber_mam_request(JabberStream *js, const char* after);

#endif /* PURPLE_JABBER_MAM_H_ */
