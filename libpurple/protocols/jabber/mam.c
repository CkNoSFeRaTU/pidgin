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

#include "jabber.h"
#include "mam.h"

struct list_s
{
	void *data;
	list_t *prev;
	list_t *next;
};

list_t * list_append(list_t *li, void *data)
{
	list_t *nli = malloc(sizeof(list_t));
	nli->data = data;
	if(!li) {
		nli->prev = nli->next = NULL;
	} else {
	        nli->prev = li;
	        nli->next = li->next;
	        if(li->next)
			li->next->prev = nli;
		li->next = nli;
	}
	return nli;
}

list_t * list_insert(list_t *li, void *data)
{
	list_t *nli = malloc(sizeof(list_t));
	nli->data = data;
	if(!li) {
		nli->prev = nli->next = NULL;
	} else {
		nli->next = li;
		nli->prev = li->prev;
		if(li->prev)
			li->prev->next = nli;
		li->prev = nli;
	}
	return nli;
}

list_t * list_delete(list_t *li, void *data)
{
	if(!li)
		return NULL;

	list_t *i = li;
	if(data) {
		i = list_get_first(li);
		while(i) {
			if(data == list_get_data(i))
				break;
			i = i->next;
		}
		if(!i)
			return li;
	}

	list_t *r = NULL;
	if(i->next) {
		i->next->prev = i->prev;
		r = i->next;
	}
	if(i->prev) {
		i->prev->next = i->next;
		r = i->prev;
	}
	free(i);
	return r;
}

inline list_t * list_get_first(list_t *li)
{
	if(!li)
		return NULL;
	while(li->prev)
		li = li->prev;
	return li;
}

inline list_t * list_get_next(list_t *li)
{
	return li->next;
}

inline void * list_get_data(list_t *li)
{
	return li->data;
}

void jabber_mam_clear(mam_t *mam)
{
	if (mam)
		return;
	
	list_t *queue_item = list_get_first(mam->queue);
	while (queue_item) {
		mam_item_t *mam_item = list_get_data(queue_item);
        
		free(mam_item->start);
		free(mam_item->end);
		free(mam_item->with);
		free(mam_item);
        
		queue_item = list_get_next(queue_item);
	}
}

void jabber_mam_add_to_queue(JabberStream *js, const char* start, const char* end, const char* with)
{
	if (!js->mam)
		return;
	
	mam_item_t *mam_item = calloc(1, sizeof(mam_item_t));

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
    
	js->mam->queue = list_append(js->mam->queue, mam_item);
	
	jabber_mam_process(js, NULL);
}

void jabber_mam_process(JabberStream *js, const char* after)
{
	if (!js->mam)
		return;
	
	if (js->mam->current && js->mam->current->completed) {
		free(js->mam->current->start);
		free(js->mam->current->end);
		free(js->mam->current->with);

		memset(js->mam->last_timestamp, 0, 32);
		
		free(js->mam->current);
		js->mam->current = NULL;
	}

	if (!js->mam->current) {
		list_t *queue_item = list_get_first(js->mam->queue);
		if (queue_item) {
			js->mam->queue = list_delete(js->mam->queue, queue_item);

			js->mam->current = list_get_data(queue_item);

			js->mam->queue = list_get_next(js->mam->queue);
		}
	}
	
	if (js->mam->current)
		jabber_mam_request(js, after);
}

void jabber_mam_request(JabberStream *js, const char* after)
{
	if (!js->mam)
		return;
	
	JabberIq *iq = jabber_iq_new_query(js, JABBER_IQ_SET, NS_XMPP_MAM);
	xmlnode *query = xmlnode_get_child(iq->node, "query");

	xmlnode *x = xmlnode_new_child(query, "x");
	xmlnode_set_namespace(x, "jabber:x:data");

	xmlnode *field = xmlnode_new_child(x, "field");
	xmlnode_set_attrib(field, "type", "hidden");
	xmlnode_set_attrib(field, "var", "FORM_TYPE");
	xmlnode *value = xmlnode_new_child(field, "value");
	xmlnode_insert_data(value, NS_XMPP_MAM, -1);

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

