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
	int getInode(char *path);//��ȡ��ǰ�ļ���inode��
	STATUS help();
	STATUS newFS(char name[]);//�����µ��ļ�ϵͳ
	STATUS sfs(char name[]);//��һ���ļ�ϵͳ
	STATUS exit();//�˳���ǰ�ļ�ϵͳ
	STATUS mkdir(char *path, char *name);//�����ļ��У�����Ϊ��ǰ·���Լ��ļ�����
	STATUS rmdir(char *path, char *name);//ɾ���ļ���,��������ͬ��
	STATUS ls(char *path);//��ʾ��ǰ�ļ����������ļ�/�ļ���
	STATUS cd(char *destPath);//�ƶ���ָ��·��
	STATUS create(char *path, char *name);//�����ļ�
	STATUS open(char *path,char *name);//�򿪵�ǰĿ¼��ĳ�ļ�
	STATUS close(char *path,char *name);//�رյ�ǰĿ¼��ĳ�ļ�
	STATUS read(char *path,char *name);//��ȡ��ǰĿ¼��ĳ�ļ�����
	STATUS write(char *path, char *name);//���ļ�д������
	STATUS deleteFile(char *path, char *name);//ɾ���ļ�/�ļ���
	STATUS rename(char *path, char *oldName,char *newName);//�������ļ�
	STATUS move(char *path, char *name, char *destPath);//�ƶ��ļ�

private:

};
