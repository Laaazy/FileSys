#pragma once

#define INODENUM 256//inode节点数
#define BLOCKNUM 1000//磁盘块数
#define BLOCKSIZE 512//磁盘块大小
#define SUPERBLOCKSIZE 5024//超级块大小
#define FILESPACESIZE 1024*1024*1024//文件存储空间大小,1G
#define NAMESIZE 16//单层文件夹下文件名长度
#define DIRSIZE 16//文件夹包含的目录项数
#define DIRNUM 32//文件夹数
#define FSSIZE FILESPACESIZE+SUPERBLOCKSIZE+30+256*216+32*324//整个程序大小
#define TYPEDIR 0//文件夹类型
#define TYPEFILE 1//文件类型

//各操作返回状态	
enum STATUS
{
	SUCCESS,ERR_PATH_FAIL,ERR_FILE_EXIST,ERR_FILE_NOT_EXIST,ERR_FILE_FAIL,ERR_TYPE_NOT_CORRECT,ERR_FILE_NOT_OPEN,ERR_FILE_ALERADY_OPEN
};

//超级块
typedef struct  superBlock
{
	//int freeInodeNUM;//可用inode数
	//int nextInode;//下一个inode节点编号
	int inodeArray[INODENUM];//inode标志数组,0为空闲，1为被占用
	//int freeBlockNum;//可用磁盘块数
	//int nextBlock;//下一磁盘块编号
	int blockArray[BLOCKNUM];//磁盘块标志数组
}superBlock;

//文件目录项结构
typedef struct dirItem {
	char fileName[NAMESIZE];//文件名
	int inodeNum;//文件inode节点号
}dirItem;

//inode节点结构
typedef struct inode {
	int fileType;//文件类型
	int isOpen;//文件是否被打开
	int fileSize;//文件大小
	int dirNumber;//文件夹对应的dirTree数组下标
	int blockHash[50];//文件块映射表，采用一级索引
}inode;

//文件夹
typedef struct dir {
	dirItem items[DIRSIZE];
	int itemNumber;//文件夹已有目录项数目
}dir;

//根节点
 struct storage {
	char diskName[30];//文件系统名
	superBlock spBlock;
	inode inodes[INODENUM];//inode数组
	dir dirTree[DIRNUM];//dir树
	char free[FILESPACESIZE];//存储文件内容
};

extern struct storage *disk;//磁盘变量
extern char fileName[30];//当前打开的文件名
extern char PATH[NAMESIZE*DIRNUM];//当前路径