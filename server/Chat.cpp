#include "Chat.h"
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <string.h>
#include <vector>
#include <libpq-fe.h>
#include <cstdio>

void Chat::chatOn()
{
	chatStatus_ = true;
}

void Chat::chatOff()
{
	chatStatus_ = false;

}

const bool Chat::getStatus() const
{
	return chatStatus_;
}

void Chat::messToParam(std::string* param, const char* data, int countParam)
{
	int j = 0;
	for (auto i = 0; i < strlen(data); i++)
	{
		if (data[i] == ' ' && (j < countParam - 1))
			j++;
		else
			param[j] += data[i];
	}
}

void Chat::regChat(const char* data, char* ans, PGconn *conn, PGresult *res)
{
	std::string userParam[3] = {};
	messToParam(userParam, data, 3);
	char snd[1000] = {};
	std::string tmp = "SELECT * FROM client WHERE login = '" + userParam[1] + "'";
	strcpy(snd, tmp.c_str());
	res = PQexec(conn, snd);
	int nrows = PQntuples(res);
	if (nrows > 0)
	{
		strcpy(ans, "0Login is busy!");
		return;
	} else 
	{
		tmp = "INSERT INTO public.client (login,firstname,lastname,email) VALUES ('" +  userParam[1] + "','" + userParam[0] + "',NULL,NULL);" 
		"UPDATE public.client_save set pass ='" + userParam[2] + "' where public.client_save.client_id = (select id from public.client where login ='" + userParam[1] + "');";
		strcpy(snd, tmp.c_str());
		PQexec(conn, snd);
		ans[0] = '1';
	}
	
}

void Chat::signIn(const char* data, char* ans, char *id, PGconn *conn, PGresult *res)
{
	std::string userParam[2] = {};
	messToParam(userParam, data, 2);
	std::string tmp = "SELECT * FROM client;";
	char snd[1000] = {};
	strcpy(snd, tmp.c_str());
	res = PQexec(conn, snd);
	int nrows = PQntuples(res);
	if (nrows <= 1)
	{
		strcpy(ans, "0Register users not found!");
		return;
	}
	bool allow = false;
	int buf = 0;
	tmp = "SELECT id, login FROM client WHERE login = '" + userParam[0] + "' and id ="
	" (select client_id from public.client_save where pass = '" + userParam[1] + "'and client_id = id)";
	strcpy(snd, tmp.c_str());
	res = PQexec(conn, snd);
	nrows = PQntuples(res);
	if (nrows > 0)
	{
		ans[0] = '1';
		strcpy(id, PQgetvalue(res, 0, 0));
	} else
	strcpy(ans, "0Wrong login/pass!");
}

void Chat::writeMessage(const char* data, char* ans, char *id, PGconn *conn, PGresult *res)
{
	std::string mess[2] = {};
	messToParam(mess, data, 2);
	char allow = '1';
	bool find = false;
	char *id_rec = {};
	std::string t1,t2;
	char snd[1000] = {};
	std::string tmp = "SELECT * FROM public.client WHERE login = '" + mess[0] + "'";
	strcpy(snd, tmp.c_str());
	res = PQexec(conn, snd);
	int nrows = PQntuples(res);
	if (nrows > 0 && mess[0] != "all")
	{
		find = true;
		id_rec = PQgetvalue(res, 0, 0);
	}
	if (id_rec != id && (find || mess[0] == "all"))
	{
		t1 = id;
		if (mess[0] == "all")
			t2 = "2";
		else
			t2 = id_rec;
		tmp = "insert into public.chat (id_sender, id_recipient) values ('" + t1 + "','" + t2 + "'); "
		"insert into public.message (chat_id, message) values (currval('chat_id_seq'::regclass), '" + mess[1] + "');";
		strcpy(snd, tmp.c_str());
		res = PQexec(conn, snd);
		ans[0] = '1';
	}
	else 
	strcpy(ans, "0Recipient is not found!");
}

void Chat::usersList(const int& connection, char *id, PGconn *conn, PGresult *res)
{
	const char end[] = "!!END!!";
	char buf[MESSAGE_LENGTH]{};
	char message[MESSAGE_LENGTH]{};
	char snd[1000] = {};
	std::string tmp = "SELECT * FROM public.client";
	strcpy(snd, tmp.c_str());
	res = PQexec(conn, snd);
	int nrows = PQntuples(res);
	for (auto i = 1; i < nrows; i++)
	{
		if (PQgetvalue(res, i, 0) == id)
			strcpy(buf, " (this account)");
		strcpy(message, "Login : '");
		strcat(message, PQgetvalue(res, i, 1));
		strcat(message, "' ");
		strcat(message, buf);
		bzero(buf, sizeof(buf));
		buf[0] = strlen(message);
		strcat(buf, message);
		write(connection, buf, sizeof(buf));
		bzero(buf, sizeof(buf));
	}
	bzero(buf, sizeof(buf));
	buf[0] = strlen(end);
	strcat(buf, end);
	write(connection, buf , sizeof(buf));
}

void Chat::dispChat(const int& connection, char *id, PGconn *conn, PGresult *res)
{
	char message[MESSAGE_LENGTH]{};
	char buf[MESSAGE_LENGTH]{};
	const char end[] = "!!END!!";
	char snd[1000] = {};
	std::string t1;
	t1 = id;
	std::string tmp = "select id_sender, id_recipient, message  from public.message left join chat on (chat_id = id)"
	"where (id_recipient = '"+ t1 + "' or id_sender = '"+ t1 + "' or id_recipient = '2')";
	strcpy(snd, tmp.c_str());
	res = PQexec(conn, snd);
	int nrows = PQntuples(res);
	for (auto i = 0; i < nrows; i++)
	{
			strcpy(message, "From : ");
			t1 = PQgetvalue(res, i, 0);
			tmp = "select login from client where id ='" + t1 + "';";
			strcpy(snd, tmp.c_str());
			strcat(message, PQgetvalue(PQexec(conn, snd),0,0));
			strcat(message, "\nTo : ");
			t1 = PQgetvalue(res, i, 1);
			tmp = "select login from client where id ='" + t1 + "';";
			strcpy(snd, tmp.c_str());
			strcat(message, PQgetvalue(PQexec(conn, snd),0,0));
			strcat(message, "\nText : ");
			strcat(message, PQgetvalue(res, i, 2));
			bzero(buf, sizeof(buf));
			buf[0] = strlen(message);
			strcat(buf, message);
			write(connection, buf, sizeof(buf));
	}
	bzero(buf, sizeof(buf));
	buf[0] = strlen(end);
	strcat(buf, end);
	write(connection, buf , sizeof(buf));
}