/**
 * @file irc.h
 *
 * purple
 *
 * Copyright (C) 2003, 2012 Ethan Blanton <elb@pidgin.im>
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

#ifndef _PURPLE_IRC_H
#define _PURPLE_IRC_H

#include <glib.h>

#ifdef HAVE_CYRUS_SASL
#include <sasl/sasl.h>
#endif

#include "ft.h"
#include "roomlist.h"
#include "sslconn.h"

#define IRC_DEFAULT_SERVER "irc.libera.chat"
#define IRC_DEFAULT_PORT 6667
#define IRC_DEFAULT_SSL_PORT 994

#define IRC_DEFAULT_CHARSET "UTF-8"
#define IRC_DEFAULT_AUTODETECT FALSE

#define IRC_DEFAULT_QUIT "Leaving."

/* By default set the command send interval to 2 seconds and allow bursting of
 * 5 commands at once.  This means, if we haven't sent a command in 10 seconds
 * we can send 5 commands immediately with no penalty.
 */
#define IRC_DEFAULT_COMMAND_INTERVAL 2
#define IRC_DEFAULT_COMMAND_MAX_BURST 5

#define IRC_BUFSIZE_INCREMENT 1024
#define IRC_MAX_BUFSIZE 16384

#define IRC_MAX_MSG_SIZE 512

#define IRC_NAMES_FLAG "irc-namelist"


enum { IRC_USEROPT_SERVER, IRC_USEROPT_PORT, IRC_USEROPT_CHARSET };
enum irc_state { IRC_STATE_NEW, IRC_STATE_ESTABLISHED };

struct irc_conn {
	PurpleAccount *account;
	GHashTable *msgs;
	GHashTable *cmds;
	char *server;
	int fd;
	guint timer;
	GHashTable *buddies;

	gboolean ison_outstanding;
	GList *buddies_outstanding;

	char *inbuf;
	int inbuflen;
	int inbufused;

	GString *motd;
	GString *names;
	struct _whois {
		char *nick;
		char *real;
		char *login;
		char *ident;
		char *host;
		char *away;
		char *server;
		char *serverinfo;
		GString *channels;
		int ircop;
		int identified;
		int idle;
		time_t signon;
	} whois;
	PurpleRoomlist *roomlist;
	PurpleSslConnection *gsc;

	gboolean quitting;

	time_t recv_time;

	GQueue *send_queue;
	time_t send_time;
	guint send_handler;
	gboolean sent_partial;

	char *mode_chars;
	char *reqnick;
	gboolean nickused;
#ifdef HAVE_CYRUS_SASL
	sasl_conn_t *sasl_conn;
	const char *current_mech;
	GString *sasl_mechs;
	gboolean mech_works;
	sasl_callback_t *sasl_cb;
#endif
};

struct irc_buddy {
	char *name;
	gboolean online;
	gboolean flag;
 	gboolean new_online_status;
	int ref;
};

typedef int (*IRCCmdCallback) (struct irc_conn *irc, const char *cmd, const char *target, const char **args);

void irc_send(struct irc_conn *irc, const char *buf);
void irc_send_len(struct irc_conn *irc, const char *buf, int len);
int irc_priority_send(struct irc_conn *irc, const char *buf);
gboolean irc_blist_timeout(struct irc_conn *irc);
gboolean irc_who_channel_timeout(struct irc_conn *irc);
void irc_buddy_query(struct irc_conn *irc);

char *irc_escape_privmsg(const char *text, gssize length);

char *irc_mirc2html(const char *string);
char *irc_mirc2txt(const char *string);

const char *irc_nick_skip_mode(struct irc_conn *irc, const char *string);

gboolean irc_ischannel(const char *string);

void irc_register_commands(void);
void irc_msg_table_build(struct irc_conn *irc);
void irc_parse_msg(struct irc_conn *irc, char *input);
char *irc_parse_ctcp(struct irc_conn *irc, const char *from, const char *to, const char *msg, int notice);
char *irc_format(struct irc_conn *irc, const char *format, ...);

void irc_msg_default(struct irc_conn *irc, const char *name, const char *from, char **args);
void irc_msg_away(struct irc_conn *irc, const char *name, const char *from, char **args);
void irc_msg_badmode(struct irc_conn *irc, const char *name, const char *from, char **args);
void irc_msg_badnick(struct irc_conn *irc, const char *name, const char *from, char **args);
void irc_msg_ban(struct irc_conn *irc, const char *name, const char *from, char **args);
void irc_msg_banfull(struct irc_conn *irc, const char *name, const char *from, char **args);
void irc_msg_banned(struct irc_conn *irc, const char *name, const char *from, char **args);
void irc_msg_chanmode(struct irc_conn *irc, const char *name, const char *from, char **args);
void irc_msg_endwhois(struct irc_conn *irc, const char *name, const char *from, char **args);
void irc_msg_features(struct irc_conn *irc, const char *name, const char *from, char **args);
void irc_msg_invite(struct irc_conn *irc, const char *name, const char *from, char **args);
void irc_msg_inviteonly(struct irc_conn *irc, const char *name, const char *from, char **args);
void irc_msg_ison(struct irc_conn *irc, const char *name, const char *from, char **args);
void irc_msg_join(struct irc_conn *irc, const char *name, const char *from, char **args);
void irc_msg_kick(struct irc_conn *irc, const char *name, const char *from, char **args);
void irc_msg_list(struct irc_conn *irc, const char *name, const char *from, char **args);
void irc_msg_luser(struct irc_conn *irc, const char *name, const char *from, char **args);
void irc_msg_mode(struct irc_conn *irc, const char *name, const char *from, char **args);
void irc_msg_motd(struct irc_conn *irc, const char *name, const char *from, char **args);
void irc_msg_names(struct irc_conn *irc, const char *name, const char *from, char **args);
void irc_msg_nick(struct irc_conn *irc, const char *name, const char *from, char **args);
void irc_msg_nickused(struct irc_conn *irc, const char *name, const char *from, char **args);
void irc_msg_nochan(struct irc_conn *irc, const char *name, const char *from, char **args);
void irc_msg_nonick(struct irc_conn *irc, const char *name, const char *from, char **args);
void irc_msg_nochangenick(struct irc_conn *irc, const char *name, const char *from, char **args);
void irc_msg_nosend(struct irc_conn *irc, const char *name, const char *from, char **args);
void irc_msg_notice(struct irc_conn *irc, const char *name, const char *from, char **args);
void irc_msg_notinchan(struct irc_conn *irc, const char *name, const char *from, char **args);
void irc_msg_notop(struct irc_conn *irc, const char *name, const char *from, char **args);
void irc_msg_part(struct irc_conn *irc, const char *name, const char *from, char **args);
void irc_msg_ping(struct irc_conn *irc, const char *name, const char *from, char **args);
void irc_msg_pong(struct irc_conn *irc, const char *name, const char *from, char **args);
void irc_msg_privmsg(struct irc_conn *irc, const char *name, const char *from, char **args);
void irc_msg_quit(struct irc_conn *irc, const char *name, const char *from, char **args);
void irc_msg_regonly(struct irc_conn *irc, const char *name, const char *from, char **args);
void irc_msg_time(struct irc_conn *irc, const char *name, const char *from, char **args);
void irc_msg_topic(struct irc_conn *irc, const char *name, const char *from, char **args);
void irc_msg_topicinfo(struct irc_conn *irc, const char *name, const char *from, char **args);
void irc_msg_unavailable(struct irc_conn *irc, const char *name, const char *from, char **args);
void irc_msg_unknown(struct irc_conn *irc, const char *name, const char *from, char **args);
void irc_msg_wallops(struct irc_conn *irc, const char *name, const char *from, char **args);
void irc_msg_whois(struct irc_conn *irc, const char *name, const char *from, char **args);
void irc_msg_who(struct irc_conn *irc, const char *name, const char *from, char **args);
#ifdef HAVE_CYRUS_SASL
void irc_msg_cap(struct irc_conn *irc, const char *name, const char *from, char **args);
void irc_msg_auth(struct irc_conn *irc, char *arg);
void irc_msg_authenticate(struct irc_conn *irc, const char *name, const char *from, char **args);
void irc_msg_authok(struct irc_conn *irc, const char *name, const char *from, char **args);
void irc_msg_authtryagain(struct irc_conn *irc, const char *name, const char *from, char **args);
void irc_msg_authfail(struct irc_conn *irc, const char *name, const char *from, char **args);
#endif

void irc_msg_ignore(struct irc_conn *irc, const char *name, const char *from, char **args);

void irc_cmd_table_build(struct irc_conn *irc);

int irc_cmd_default(struct irc_conn *irc, const char *cmd, const char *target, const char **args);
int irc_cmd_away(struct irc_conn *irc, const char *cmd, const char *target, const char **args);
int irc_cmd_ctcp(struct irc_conn *irc, const char *cmd, const char *target, const char **args);
int irc_cmd_ctcp_action(struct irc_conn *irc, const char *cmd, const char *target, const char **args);
int irc_cmd_ctcp_version(struct irc_conn *irc, const char *cmd, const char *target, const char **args);
int irc_cmd_invite(struct irc_conn *irc, const char *cmd, const char *target, const char **args);
int irc_cmd_join(struct irc_conn *irc, const char *cmd, const char *target, const char **args);
int irc_cmd_kick(struct irc_conn *irc, const char *cmd, const char *target, const char **args);
int irc_cmd_list(struct irc_conn *irc, const char *cmd, const char *target, const char **args);
int irc_cmd_mode(struct irc_conn *irc, const char *cmd, const char *target, const char **args);
int irc_cmd_names(struct irc_conn *irc, const char *cmd, const char *target, const char **args);
int irc_cmd_nick(struct irc_conn *irc, const char *cmd, const char *target, const char **args);
int irc_cmd_op(struct irc_conn *irc, const char *cmd, const char *target, const char **args);
int irc_cmd_privmsg(struct irc_conn *irc, const char *cmd, const char *target, const char **args);
int irc_cmd_part(struct irc_conn *irc, const char *cmd, const char *target, const char **args);
int irc_cmd_ping(struct irc_conn *irc, const char *cmd, const char *target, const char **args);
int irc_cmd_quit(struct irc_conn *irc, const char *cmd, const char *target, const char **args);
int irc_cmd_quote(struct irc_conn *irc, const char *cmd, const char *target, const char **args);
int irc_cmd_query(struct irc_conn *irc, const char *cmd, const char *target, const char **args);
int irc_cmd_remove(struct irc_conn *irc, const char *cmd, const char *target, const char **args);
int irc_cmd_service(struct irc_conn *irc, const char *cmd, const char *target, const char **args);
int irc_cmd_time(struct irc_conn *irc, const char *cmd, const char *target, const char **args);
int irc_cmd_topic(struct irc_conn *irc, const char *cmd, const char *target, const char **args);
int irc_cmd_wallops(struct irc_conn *irc, const char *cmd, const char *target, const char **args);
int irc_cmd_whois(struct irc_conn *irc, const char *cmd, const char *target, const char **args);
int irc_cmd_whowas(struct irc_conn *irc, const char *cmd, const char *target, const char **args);

PurpleXfer *irc_dccsend_new_xfer(PurpleConnection *gc, const char *who);
void irc_dccsend_send_file(PurpleConnection *gc, const char *who, const char *file);
void irc_dccsend_recv(struct irc_conn *irc, const char *from, const char *msg);
#endif /* _PURPLE_IRC_H */
