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

#include "../util.h"

gint LLVMFuzzerTestOneInput(const guint8 *data, size_t size);

gint
LLVMFuzzerTestOneInput(const guint8 *data, size_t size) {
	gchar *malicious_mime = g_new0(gchar, size + 1);
	gchar *result = NULL;

	memcpy(malicious_mime, data, size);
	malicious_mime[size] = '\0';

	result = purple_mime_decode_field(malicious_mime);
	g_free(result);

	g_free(malicious_mime);

	return 0;
}
