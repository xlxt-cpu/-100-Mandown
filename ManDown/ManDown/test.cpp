/*
#include <stdio.h>
#include <conio.h>
#include <mysql.h>

#pragma comment(lib, "libmysql.lib")
void MysqlTest(int argc, char *argv[])
{
	char host[] = "localhost";
	char szDatabase[] = "mysql";	//mysql已有的数据库名
	char username[] = "root";
	char password[] = "20010422";	//mysql管理员密码
	unsigned int port = 3306;
	MYSQL *conn;
	if ((conn = mysql_init((MYSQL*)0))
		&& (mysql_options(conn, MYSQL_SET_CHARSET_NAME, "gbk") == 0)
		&& mysql_real_connect(conn, host, username, password, szDatabase, port, NULL, 0))
		printf("连接数据库成功！\n");
	else
	{
		printf("连接数据库失败！\n");
		mysql_close(conn);
		getch();
	}
	mysql_close(conn);
	getch();
}
*/

