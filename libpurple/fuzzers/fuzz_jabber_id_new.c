/* purple
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02111-1301  USA
 */

#include <string.h>

#include <glib.h>

#include <purple.h>

#include "../protocols/jabber/jutil.h"

gint LLVMFuzzerTestOneInput(const guint8 *data, size_t size);

gint
LLVMFuzzerTestOneInput(const guint8 *data, size_t size) {
	JabberID *jid = NULL;
	gchar *malicious_jid = NULL;

	if(!g_utf8_validate_len((const gchar *)data, size, NULL)) {
		return 0;
	}

	malicious_jid = g_new0(gchar, size + 1);
	memcpy(malicious_jid, data, size);
	malicious_jid[size] = '\0';

	jid = jabber_id_new(malicious_jid);

	jabber_id_free(jid);

	g_free(malicious_jid);

	return 0;
}

