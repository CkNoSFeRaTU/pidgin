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

#include "internal.h"
#include "request.h"
#include "debug.h"

#include "jabber.h"
#include "mam.h"

void jabber_mam_clear(mam_t *mam)
{
	GList *queue_item;
	mam_item_t *mam_item;

	if (mam)
		return;

	queue_item = g_list_first(mam->queue);
	while (queue_item) {
		mam_item = queue_item->data;

		free(mam_item->start);
		free(mam_item->end);
		free(mam_item->with);
		free(mam_item);

		queue_item = g_list_next(queue_item);
	}
}

void jabber_mam_add_to_queue(JabberStream *js, const char* start, const char* end, const char* with)
{
	mam_item_t *mam_item;

	if (!js->mam)
		return;

	purple_debug_info("jabber", "MAM add to queue\n");

	mam_item = calloc(1, sizeof(mam_item_t));

	if (start) {
		mam_item->start = calloc(1, strlen(start) + 1);
		strcpy(mam_item->start, start);
	}

	if (end) {
		mam_item->end = calloc(1, strlen(end) + 1);
		strcpy(mam_item->end, end);
	}

	if (with) {
		mam_item->with = calloc(1, strlen(with) + 1);
		strcpy(mam_item->with, with);
	}

	js->mam->queue = g_list_append(js->mam->queue, mam_item);

	jabber_mam_process(js, NULL);
}

void jabber_mam_process(JabberStream *js, const char* after)
{
	GList *queue_item;

	if (!js->mam)
		return;

	purple_debug_info("jabber", "MAM process\n");

	if (js->mam->current && js->mam->current->completed) {
		free(js->mam->current->start);
		free(js->mam->current->end);
		free(js->mam->current->with);

		memset(js->mam->last_timestamp, 0, sizeof(js->mam->last_timestamp));

		free(js->mam->current);
		js->mam->current = NULL;
	}

	if (!js->mam->current) {
		queue_item = g_list_first(js->mam->queue);
		if (queue_item) {
			js->mam->queue = g_list_remove(js->mam->queue, queue_item);

			js->mam->current = queue_item->data;

			js->mam->queue = g_list_next(js->mam->queue);
		}
	}

	if (js->mam->current)
		jabber_mam_request(js, after);
}

void jabber_mam_request(JabberStream *js, const char* after)
{
	JabberIq *iq;
	xmlnode *x, *query, *field, *value;

	if (!js->mam)
		return;

	if (!js->mam->ns) {
		purple_debug_info("jabber", "MAM no namespace, skipping request\n");
		jabber_mam_clear(js->mam);

		return;
	}

	purple_debug_info("jabber", "MAM request\n");

	iq = jabber_iq_new_query(js, JABBER_IQ_SET, js->mam->ns);
	query = xmlnode_get_child(iq->node, "query");

	x = xmlnode_new_child(query, "x");
	xmlnode_set_namespace(x, "jabber:x:data");
	xmlnode_set_attrib(x, "type", "submit");

	field = xmlnode_new_child(x, "field");
	xmlnode_set_attrib(field, "type", "hidden");
	xmlnode_set_attrib(field, "var", "FORM_TYPE");
	value = xmlnode_new_child(field, "value");
	xmlnode_insert_data(value, js->mam->ns, -1);

	if (after) {
		xmlnode *set = xmlnode_new_child(query, "set");
		xmlnode_set_namespace(set, NS_RSM);

		value = xmlnode_new_child(set, "after");
		xmlnode_insert_data(value, after, -1);
	}

	if (js->mam->current->start) {
		field = xmlnode_new_child(x, "field");
		xmlnode_set_attrib(field, "var", "start");

		value = xmlnode_new_child(field, "value");
		xmlnode_insert_data(value, js->mam->current->start, -1);
	}

	if (js->mam->current->end) {
		field = xmlnode_new_child(x, "field");
		xmlnode_set_attrib(field, "var", "end");

		value = xmlnode_new_child(field, "value");
		xmlnode_insert_data(value, js->mam->current->end, -1);
	}

	if (js->mam->current->with) {
		field = xmlnode_new_child(x, "field");
		xmlnode_set_attrib(field, "var", "with");

		value = xmlnode_new_child(field, "value");
		xmlnode_insert_data(value, js->mam->current->with, -1);
	}

	jabber_iq_send(iq);
}

