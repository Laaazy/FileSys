#pragma once

#include <iostream>
#include <iomanip>
#include <string>
#include <conio.h>
#include "defines.h"

using namespace std;

class operation
{
public:
	int getInode(char *path);//获取当前文件的inode号
	STATUS help();
	STATUS newFS(char name[]);//创建新的文件系统
	STATUS sfs(char name[]);//打开一个文件系统
	STATUS exit();//退出当前文件系统
	STATUS mkdir(char *path, char *name);//创建文件夹，参数为当前路径以及文件夹名
	STATUS rmdir(char *path, char *name);//删除文件夹,参数含义同上
	STATUS ls(char *path);//显示当前文件夹下所有文件/文件夹
	STATUS cd(char *destPath);//移动到指定路径
	STATUS create(char *path, char *name);//创建文件
	STATUS open(char *path,char *name);//打开当前目录下某文件
	STATUS close(char *path,char *name);//关闭当前目录下某文件
	STATUS read(char *path,char *name);//读取当前目录下某文件内容
	STATUS write(char *path, char *name);//往文件写入内容
	STATUS deleteFile(char *path, char *name);//删除文件/文件夹
	STATUS rename(char *path, char *oldName,char *newName);//重命名文件
	STATUS move(char *path, char *name, char *destPath);//移动文件

private:

};
