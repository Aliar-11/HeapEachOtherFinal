#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<stdlib.h>
#include<graphics.h>
#include<easyx.h>

//////////////////////////�ļ�˵��//////////////////////////
/*
AdminSignIn.txt������Ա�˺š�����
CommonUserList.txt����ͨ�û��˺ţ��ֻ��ţ������롢סַ������
ApplicationList.txt������ע������û���Ϣ
ItemList.txt����Ʒ�б�
TypeList.txt����Ʒ�����б�
*/

#define Num 30    //���ɴ���ļ�¼��

int ItemNum = 0;    //��ǰ��Ʒ��
int TypeNum = 0;    //��ǰ������
int CommonUserNum = 0;    //��ǰ��ͨ�û���
int RegisNum = 0;    //����׼��ע��������

int BackAdmin = -1;    //Ϊ-1����ת��AdminPage��Ϊ-1����ת��CommonUserPage

//��Ʒ�ṹ��
typedef struct Item
{
	char i_Name[64];    //��Ʒ����
	char i_Type[64];    //��Ʒ����
	char i_Note[64];    //��Ʒ˵�������͵�����
	char i_Tel[64];    //��ϵ�˵绰
	char i_Addr[64];    //��ϵ�˵�ַ
	char i_Email[64];    //��ϵ������
}Item;

Item ItemArry[Num],ItemFound[Num];

//��Ʒ���ͽṹ��
typedef struct Type
{
	char t_Name[64];
	char t_Attrib[64];
}Type;

Type TypeArry[Num];

//����Ա�ṹ��
typedef struct Admin
{
	char UserName[64];
	char PassWord[64];
}Admin;

Admin AdminUser;

//��ͨ�û��ṹ��
typedef struct CommonUser
{
	char UserName[64];    //�ֻ�����Ϊ�û���
	char PassWord[64];
	char Addr[64];
	char Email[64];
}CommonUser;

CommonUser User[Num],CurrentUser,Regis[Num];

//��ʾȫ�����ݵ�ҳ����ת��ʶ
int FlagAI = -1;
//��ѯ�ҵ�ҳ����ת��ʶ
int FlagFPT = -1;

//ǰ������
void AdminPage();    //����Ա����
void AdminSignIn();    //����Ա��½����
void CommonUserSignIn();    //��ͨ�û���¼����
void CommonUserPage();    //��ͨ�û�����
void Registration();    //���û�ע������ҳ��
void SaveItem();    //����ItemList.txt�ļ�
void SaveType();    //����Type.txt�ļ�
void ReadInfor();    //��ȡItemList.txt��TypeList.txt
void SaveLastItem();    //����������ӵ�һ����Ʒ��Item.txt
void SaveLastType();    //����������ӵ�һ�����͵�Type.txt
void SaveApplication();    //��������ע��Ĵ�����û�
void SaveNewCommonUser(int x);    //����ͨ����˵����û�
void ShowAllType();    //��ʾȫ������
int ShowUserItem();    //��ʾ��ǰ�û���ӵ���Ʒ
void ManageItem();    //��ɾ�����Ʒ
void ManageType();    //��ɾ������ͣ�����Ա���ܣ�
void AddType();    //�������
void DeleteType();    //ɾ������
void ShowApplication();    //��ʾ���û�������Ϣ
void ItemPrint(int tot);    //��ӡ��Ʒ��Ϣ
void AllItemPrint();    //��ӡ������Ʒ��Ϣ
void ShowAllItem();    //��ӡ������Ʒ
void TypePrint();    //��ӡ������Ϣ
int DeletePosition();    //����Ҫɾ����Ʒ���±�
void DeleteItem();    //ɾ����Ʒ
void SearchItem();    //������Ʒ
void AddItem();    //�����Ʒ



/*
	//////////////////////ʵ��ͼ�ν���//////////////////////
*/


//��ҳ
void FirstPage()
{
	//��ȡ����
	ReadInfor();

	//���ڴ�С
	initgraph(760, 760);
	//������1
	MOUSEMSG m1;
	//���ñ���ͼƬ
	cleardevice();
	IMAGE img;
	loadimage(&img, "./background.png", 760, 760);
	putimage(0, 0, &img);
	setbkmode(TRANSPARENT);

	setfillcolor(RGB(193, 193, 193));
	settextcolor(BLACK);

	//�����
	char Title[20] = { "�����������ϵͳ" };
	settextstyle(60, 0, "����");
	outtextxy(150, 100, Title);

	//4����ť�ı���
	fillrectangle(230, 385, 560, 430);
	fillrectangle(230, 445, 560, 490);
	fillrectangle(230, 505, 560, 550);
	fillrectangle(230, 565, 560, 610);
	setbkmode(TRANSPARENT);
	//4����ť������
	settextstyle(25, 0, "����");
	char FirstButton0[20] = { "���û�ע������" };
	char FirstButton1[20] = { "����Ա��������" };
	char FirstButton2[20] = { "��ͨ�û���������" };
	char FirstButton3[20] = { "�˳�����" };
	outtextxy(305, 390, FirstButton0);
	outtextxy(305, 450, FirstButton1);
	outtextxy(290, 510, FirstButton2);
	outtextxy(340, 570, FirstButton3);

	
	//���ð�ť����
	while (true)
	{
		m1 = GetMouseMsg();
		if (m1.x >= 230 && m1.x <= 560 && m1.y >= 385 && m1.y <= 430)
		{
			setlinecolor(RED);
			rectangle(230, 385, 560, 430);
			if (m1.uMsg == WM_LBUTTONDOWN)
			{
				//����ע��
				Registration();
			}
		}
		else if (m1.x >= 230 && m1.x <= 560 && m1.y >= 445 && m1.y <= 490)    //����Ա����
		{
			setlinecolor(RED);
			rectangle(230, 445, 560, 490);
			if (m1.uMsg == WM_LBUTTONDOWN)
			{			
				AdminSignIn();    //����Ա��¼
			}
		}
		else if (m1.x >= 230 && m1.x <= 560 && m1.y >= 505 && m1.y <= 550)    //��ͨ�û�����
		{
			setlinecolor(RED);
			rectangle(230, 505, 560, 550);
			if (m1.uMsg == WM_LBUTTONDOWN)
			{
				CommonUserSignIn();    //��ͨ�û���¼
			}
		}
		else if (m1.x >= 230 && m1.x <= 560 && m1.y >= 565 && m1.y <= 610)    //�˳�
		{
			setlinecolor(RED);
			rectangle(230, 565, 560, 610);
			if (m1.uMsg == WM_LBUTTONDOWN)
			{
				exit(0);
				strcpy(CurrentUser.UserName,"");    //���õ�ǰ�û���Ϊ��
			}
		}
		else    //��겻���κΰ�ť����ͣ
		{
			setlinecolor(WHITE);
			rectangle(230, 385, 560, 430);
			rectangle(230, 445, 560, 490);
			rectangle(230, 505, 560, 550);
			rectangle(230, 565, 560, 610);
		}
	}
}

//����Ա����
void AdminPage()
{
	initgraph(760, 760);   	
	cleardevice();
	//�ڶ���������
	MOUSEMSG m2;
	cleardevice();
	IMAGE img;
	loadimage(&img, "./background.png", 760, 760);
	putimage(0, 0, &img);
	setfillcolor(RGB(193, 193, 193));
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);

	char SecondTitle1[5] = { "��" };
	char SecondTitle2[5] = { "��" };
	char SecondTitle3[5] = { "Ա" };
	char SecondTitle4[5] = { "��" };
	char SecondTitle5[5] = { "��" };
	char SecondTitle6[5] = { "��" };
	char SecondTitle7[5] = { "��" };


	settextstyle(30, 0, "����");
	outtextxy(50, 150, SecondTitle1);
	outtextxy(50, 210, SecondTitle2);
	outtextxy(50, 270, SecondTitle3);
	outtextxy(50, 330, SecondTitle4);
	outtextxy(50, 390, SecondTitle5);
	outtextxy(50, 450, SecondTitle6);
	outtextxy(50, 510, SecondTitle7);
	setbkmode(TRANSPARENT);

	//4����ť�ı���
	fillrectangle(230, 110, 560, 155);
	fillrectangle(230, 240, 560, 285);
	fillrectangle(230, 370, 560, 415);
	fillrectangle(230, 500, 560, 545);
	setbkmode(TRANSPARENT);

	//4����ť������
	settextstyle(30, 0, "����");
	char SecondSelect0[20] = { "�û�ע������" };
	char SecondSelect1[20] = { "��ʾ��������" };
	char SecondSelect2[20] = { "���/�޸�����" };
	char SecondSelect3[20] = { "����" };
	outtextxy(300, 115, SecondSelect0);
	outtextxy(300, 245, SecondSelect1);
	outtextxy(300, 375, SecondSelect2);
	outtextxy(360, 505, SecondSelect3);

	//����������ѡ���������
	while (true)
	{
		m2 = GetMouseMsg();//��ȡ������
		if (m2.x >= 230 && m2.x <= 560 && m2.y >= 110 && m2.y <= 155)
		{
			setlinecolor(RED);
			rectangle(230, 110, 560, 155);
			if (m2.uMsg == WM_LBUTTONDOWN)
			{
				//�û�ע������
				cleardevice();
				BackAdmin = 1;
				ShowApplication();
			}
		}
		else if (m2.x >= 230 && m2.x <= 560 && m2.y >= 240 && m2.y <= 285)
		{
			setlinecolor(RED);
			rectangle(230, 240, 560, 285);
			if (m2.uMsg == WM_LBUTTONDOWN)
			{
				//��ʾ��������
				cleardevice();
				ShowAllType();
			}
		}
		else if (m2.x >= 230 && m2.x <= 560 && m2.y >= 370 && m2.y <= 415)
		{
			setlinecolor(RED);
			rectangle(230, 370, 560, 415);
			if (m2.uMsg == WM_LBUTTONDOWN)
			{
				cleardevice();
				ManageType();
			}
		}
		else if (m2.x >= 230 && m2.x <= 560 && m2.y >= 500 && m2.y <= 545)
		{
			setlinecolor(RED);
			rectangle(230, 500, 560, 545);
			if (m2.uMsg == WM_LBUTTONDOWN)
			{
				//����
				cleardevice();
				FirstPage();
			}
		}
		//��겻���κΰ�ť����ͣ
		else
		{
			setlinecolor(WHITE);
			rectangle(230, 240, 560, 285);
			rectangle(230, 370, 560, 415);
			rectangle(230, 500, 560, 545);
		}
	}

}

//��ͨ�û�����
void CommonUserPage()
{
	initgraph(760, 760);
	cleardevice();
	//�����3��������
	MOUSEMSG m3;
	cleardevice();
	IMAGE img;
	loadimage(&img, "./background.png", 760, 760);
	putimage(0, 0, &img);
	setfillcolor(RGB(193, 193, 193));
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);

	char SecondTitle1[5] = { "��" };
	char SecondTitle2[5] = { "ͨ" };
	char SecondTitle3[5] = { "��" };
	char SecondTitle4[5] = { "��" };
	char SecondTitle5[5] = { "��" };
	char SecondTitle6[5] = { "��" };
	char SecondTitle7[5] = { "��" };
	char SecondTitle8[5] = { "��" };

	settextstyle(30, 0, "����");
	outtextxy(50, 150, SecondTitle1);
	outtextxy(50, 210, SecondTitle2);
	outtextxy(50, 270, SecondTitle3);
	outtextxy(50, 330, SecondTitle4);
	outtextxy(50, 390, SecondTitle5);
	outtextxy(50, 450, SecondTitle6);
	outtextxy(50, 510, SecondTitle7);
	outtextxy(50, 570, SecondTitle8);
	setbkmode(TRANSPARENT);

	//������ť�ı���
	fillrectangle(230, 240, 560, 285);
	fillrectangle(230, 370, 560, 415);
	fillrectangle(230, 500, 560, 545);
	setbkmode(TRANSPARENT);
	//������ť������
	settextstyle(30, 0, "����");
	char ThirdSelect1[20] = { "��ʾ������Ʒ" };
	char ThirdSelect2[20] = { "��ɾ������Ʒ" };
	char ThirdSelect3[20] = { "����" };
	outtextxy(300, 245, ThirdSelect1);
	outtextxy(300, 375, ThirdSelect2);
	outtextxy(370, 505, ThirdSelect3);

	//����������ѡ���������
	while (1)
	{
		m3 = GetMouseMsg();//��ȡ������
		if (m3.x >= 230 && m3.x <= 560 && m3.y >= 240 && m3.y <= 285)
		{
			setlinecolor(RED);
			rectangle(230, 240, 560, 285);
			if (m3.uMsg == WM_LBUTTONDOWN)
			{
				//��ʾ������Ʒ
				cleardevice();
				ShowAllItem();
			}
		}
		else if (m3.x >= 230 && m3.x <= 560 && m3.y >= 370 && m3.y <= 415)
		{
			setlinecolor(RED);
			rectangle(230, 370, 560, 415);
			if (m3.uMsg == WM_LBUTTONDOWN)
			{
				//��ɾ������
				cleardevice();
				ManageItem();
			}
		}
		else if (m3.x >= 230 && m3.x <= 560 && m3.y >= 500 && m3.y <= 545)//ѡ��3��������
		{
			setlinecolor(RED);
			rectangle(230, 500, 560, 545);
			if (m3.uMsg == WM_LBUTTONDOWN)
			{
				cleardevice();
				FirstPage();
			}
		}
		//��겻���κΰ�ť����ͣ
		else
		{
			setlinecolor(WHITE);
			rectangle(230, 240, 560, 285);
			rectangle(230, 370, 560, 415);
			rectangle(230, 500, 560, 545);
		}
	}
}

/*
	//////////////////////�û���¼��ע��//////////////////////
*/

//����Ա��¼
void AdminSignIn()
{
	//��ȡ����Ա�û���������
	FILE* fp = fopen("AdminSignIn.txt", "r");
	if (fp == NULL)
	{
		return;
	}
	fscanf(fp, "%s\t%s\t\n", AdminUser.UserName, AdminUser.PassWord);
	fclose(fp);

	char ReceAcctNumber[64];
	TCHAR InputAcct[] = _T("�������û���");
	InputBox(ReceAcctNumber, 12, InputAcct);

	char ReceAcctPassWord[64];
	TCHAR InputPass[] = _T("����������");
	InputBox(ReceAcctPassWord, 7, InputPass);

	//����û��������붼��ȷ�Ž������Ա���棬���򵯴���ʾ����
	if (strcmp(AdminUser.UserName, ReceAcctNumber) == 0 && strcmp(AdminUser.PassWord, ReceAcctPassWord) == 0)
	{
		cleardevice();
		AdminPage();    //����Ա����
	}
	else
	{
		HWND SignError = GetHWnd();
		int isok = MessageBox(SignError, "�û������������!", "��ʾ", MB_OK);
	}
}

//��ͨ�û���¼
void CommonUserSignIn()
{
	/*
	//��ȡ�û���������
	FILE* fp = fopen("CommonUserSignIn.txt", "r");
	if (fp == NULL)
	{
		return;
	}
	for (int i = 0; i < Num; i++)
	{
		fscanf(fp, "%s\t%s\t\n",
			User[i].UserName,
			User[i].PassWord);
	}
	int UserNum = 0;
	for (int k = 0; k < Num; k++)
	{
		if (strlen(User[k].UserName) != 0)
		{
			UserNum++;
		}
	}
	fclose(fp);
	*/
	char ReceAcctNumber[64];
	TCHAR InputAcct[] = _T("�������û���");
	InputBox(ReceAcctNumber, 12, InputAcct);

	char ReceAcctPassWord[64];
	TCHAR InputPass[] = _T("����������");
	InputBox(ReceAcctPassWord, 7, InputPass);

	bool flag = false;    //��¼�Ƿ��ҵ�һ����ȷ���û���������
	for (int i = 0; i < CommonUserNum; i++)
	{
		if (strcmp(User[i].UserName, ReceAcctNumber) == 0 && strcmp(User[i].PassWord, ReceAcctPassWord) == 0)
		{
			flag = true;
			CurrentUser = User[i];    //��¼��ǰ�û��ֻ���
			cleardevice();
			CommonUserPage();    //��ͨ�û�����
		}
	}
	if (!flag)
	{
		HWND SignError = GetHWnd();
		int isok = MessageBox(SignError, "�û������������!", "��ʾ", MB_OK);
	}
}

//���û�ע��
void Registration() {
	while (true)
	{
		TCHAR InputWindow0[] = _T("�����������ֻ�����Ϊ�û���");
		InputBox(Regis[RegisNum].UserName, 12, InputWindow0);
		//�ж��Ƿ������ظ�
		bool Flag = false;
		for (int i = 0; i < CommonUserNum; i++)
		{
			if (strcmp(Regis[RegisNum].UserName, User[i].UserName) == 0)
			{
				Flag = true;
				break;
			}
		}
		if (Flag)
		{
			HWND hwndInput1 = GetHWnd();
			int isok = MessageBox(hwndInput1, "���ֻ�����ע�ᣬ����������", "��ʾ", MB_OK);
		}
		else
		{
			break;
		}
	}

	//��������
	TCHAR InputWindow2[] = _T("��������λ����");
	InputBox(Regis[RegisNum].PassWord, 7, InputWindow2);
	//�����ַ
	TCHAR InputWindow3[] = _T("���������ĵ�ַ");
	InputBox(Regis[RegisNum].Addr, 16, InputWindow3);
	//��������
	TCHAR InputWindow4[] = _T("��������������");
	InputBox(Regis[RegisNum].Email, 21, InputWindow4);
	//����������Աδ����ϢApplicationList.txt
	SaveApplication();

	HWND hwndInput2 = GetHWnd();
	int isok = MessageBox(hwndInput2, "ע�������ѷ��ͣ������ĵȴ�����Ա���", "��ʾ", MB_OK);
}


/*
	//////////////////////��������//////////////////////
*/

//��ȡ�ļ�
void ReadInfor()
{
	//��ȡ��Ʒ�б�
	FILE* fp = fopen("ItemList.txt", "r");
	if (fp == NULL)
	{
		return;
	}

	for (int i = 0; i < Num; i++)
	{
		fscanf(fp, "%s\t%s\t%s\t%s\t%s\t%s\t\n",
			ItemArry[i].i_Name,
			ItemArry[i].i_Type,
			ItemArry[i].i_Note,
			ItemArry[i].i_Tel,
			ItemArry[i].i_Addr,
			ItemArry[i].i_Email);
	}
	//��¼��Ʒ����
	for (int k = 0; k < Num; k++)
	{
		if (strlen(ItemArry[k].i_Name) != 0)
		{
			ItemNum++;
		}
	}
	fclose(fp);

	//��ȡ��Ʒ�����б�
	fp = fopen("TypeList.txt", "r");
	if (fp == NULL)
	{
		return;
	}
	for (int i = 0; i < Num; i++)
	{
		fscanf(fp, "%s\t%s\n",
			TypeArry[i].t_Name,
			TypeArry[i].t_Attrib);
	}
	//��¼��������
	for (int k = 0; k < Num; k++)
	{
		if (strlen(TypeArry[k].t_Name) != 0)
		{
			TypeNum++;
		}
	}
	fclose(fp);

	//��ȡ�û��б�
	fp = fopen("CommonUserList.txt", "r");
	if (fp == NULL)
	{
		return;
	}
	for (int i = 0; i < Num; i++)
	{
		fscanf(fp, "%s\t%s\t%s\t%s\n",
			User[i].UserName,
			User[i].PassWord,
			User[i].Addr,
			User[i].Email);
	}
	//��¼��ͨ�û�����
	for (int k = 0; k < Num; k++)
	{
		if (strlen(User[k].UserName) != 0)
		{
			CommonUserNum++;
		}
	}
	fclose(fp);

	//��ȡע�������б�
	fp = fopen("ApplicationList.txt", "r");
	if (fp == NULL)
	{
		return;
	}
	for (int i = 0; i < Num; i++)
	{
		fscanf(fp, "%s\t%s\t%s\t%s\n",
			Regis[i].UserName,
			Regis[i].PassWord,
			Regis[i].Addr,
			Regis[i].Email);
	}
	//��¼��ͨ�û�����
	for (int k = 0; k < Num; k++)
	{
		if (strlen(Regis[k].UserName) != 0)
		{
			RegisNum++;
		}
	}
	fclose(fp);
}

//�����µ�һ��ע��������ApplicationList.txt
void SaveApplication() 
{
	if (strlen(Regis[RegisNum].UserName) != 0)
	{
		FILE* fp = fopen("ApplicationList.txt", "a");
		if (fp == NULL)
		{
			return;
		}
		fprintf(fp, "%s\t%s\t%s\t%s\n",
			Regis[RegisNum].UserName,
			Regis[RegisNum].PassWord,
			Regis[RegisNum].Addr,
			Regis[RegisNum].Email);
		fclose(fp);
		//���������Ʒ����++
		RegisNum++;
	}
}

//����Item.txt�ļ�
void SaveItem()
{
	remove("ItemList.txt");
	//�����Ϊ�ղ�д���ļ�
	FILE* fp = fopen("ItemList.txt", "a");
	if (fp == NULL)
	{
		return;
	}
	for (int i = 0; i < ItemNum; i++)
	{
		if (strlen(ItemArry[i].i_Name) != 0)
		{
			fprintf(fp, "%s\t%s\t%s\t%s\t%s\t%s\n",
				ItemArry[i].i_Name,
				ItemArry[i].i_Type,
				ItemArry[i].i_Note,
				ItemArry[i].i_Tel,
				ItemArry[i].i_Addr,
				ItemArry[i].i_Email);
		}
	}
	fclose(fp);
}

//����������ӵ�һ����Ʒ���ļ�
void SaveLastItem()
{
	//��Ϊ�ղ�д���ļ�
	if (strlen(ItemArry[ItemNum].i_Name) != 0)
	{
		FILE* fp = fopen("ItemList.txt", "a");
		if (fp == NULL)
		{
			return;
		}
		fprintf(fp, "%s\t%s\t%s\t%s\t%s\t%s\n",
			ItemArry[ItemNum].i_Name,
			ItemArry[ItemNum].i_Type,
			ItemArry[ItemNum].i_Note,
			ItemArry[ItemNum].i_Tel,
			ItemArry[ItemNum].i_Addr,
			ItemArry[ItemNum].i_Email);
		fclose(fp);
		//���������Ʒ����++
		ItemNum++;
	}
}

//����Ҫɾ����Ʒ���±�
int DeletePosition()
{
	//�����û�����
	char ReceInput[64];
	TCHAR InputWindowFI[] = _T("������Ҫɾ������Ʒ����\nע�⣺��ֻ��ɾ���Լ���ӵ���Ʒ");
	InputBox(ReceInput, 20, InputWindowFI);

	//����ѭ�����ң�һ����ͬ������ѭ��
	for (int w = 0; w < ItemNum; w++)
	{
		if (strcmp(ItemArry[w].i_Name, ReceInput) == 0 
			&& strcmp(ItemArry[w].i_Tel, CurrentUser.UserName) == 0)  
		{
			return w;
		}
	}
	//û�ҵ��ͷ���-1
	return -1;
}

//�����Ʒ
void AddItem()
{
	//���ܴ�����Ʒ�����û�����ͬ�ļ�¼
	//ͨ��ѭ��������Ƿ����ظ���
	char Attrib[64];

	//������Ʒ����
	while (true)
	{
		TCHAR InputWindow0[] = _T("��������Ʒ����");
		InputBox(ItemArry[ItemNum].i_Name, 16, InputWindow0,0,0,0,0,false);
		//�ж��Ƿ������ظ�
		bool Flag = false;
		for (int i = 0; i < ItemNum; i++)
		{
			if (strcmp(ItemArry[ItemNum].i_Name, ItemArry[i].i_Name) == 0
				&& strcmp(ItemArry[ItemNum].i_Tel, ItemArry[i].i_Tel) == 0)
			{
				Flag = true;
				break;
			}
		}
		if (Flag)
		{
			HWND hwndInput1 = GetHWnd();
			int isok = MessageBox(hwndInput1, "����Ʒ�Ѵ��ڣ�����������", "��ʾ", MB_OK);
		}
		else
		{
			break;
		}
	}

	//��������
	TypePrint();
	while (true)
	{
		TCHAR InputWindow1[] = _T("��������Ʒ����");
		InputBox(ItemArry[ItemNum].i_Type, 16, InputWindow1,NULL,NULL,0,0,false);

		//�ж�����������Ƿ����
		bool Flag = false;
		for (int i = 0; i < TypeNum; i++)
		{
			if (strcmp(ItemArry[ItemNum].i_Type, TypeArry[i].t_Name) == 0)
			{
				Flag = true;
				strcpy(Attrib, TypeArry[i].t_Attrib);
				break;
			}
		}
		if (!Flag)
		{
			HWND hwndInput1 = GetHWnd();
			int isok = MessageBox(hwndInput1, "�����Ͳ����ڣ�����������", "��ʾ", MB_OK);
			ManageItem();
		}
		else
		{
			break;
		}
	}

	//��������˵��
	TCHAR InputWindow2[] = _T("��������Ʒ˵������ʽ�������������ڣ�XXX��");
	InputBox(ItemArry[ItemNum].i_Note, 21, InputWindow2, NULL, NULL, 0, 0, false);

	strcpy(ItemArry[ItemNum].i_Tel, CurrentUser.UserName);
	strcpy(ItemArry[ItemNum].i_Addr, CurrentUser.Addr);
	strcpy(ItemArry[ItemNum].i_Email, CurrentUser.Email);

	//��������
	SaveLastItem();
}

//ɾ��ĳ����Ʒ����Ϣ
void DeleteItem()
{
	if (ShowUserItem() == -1)
	{
		HWND hndtipsD = GetHWnd();
		int isok = MessageBox(hndtipsD, "����δ�����Ʒ��û�п���ɾ������Ʒ!", "��ʾ", MB_OK);
		return;
	}
	int TempDelt = DeletePosition();
	if (TempDelt == -1)    //û�ҵ�
	{
		//��ȡ���ھ��
		HWND hndtipsD = GetHWnd();
		int isok = MessageBox(hndtipsD, "�����ڸ���Ʒ!", "��ʾ", MB_OK);
	}
	else    //�ҵ��ˣ�ȷ���Ƿ�ɾ��
	{
		HWND hndtipsDS = GetHWnd();
		int isok = MessageBox(hndtipsDS, "ȷ��ɾ����", "��ʾ", MB_OKCANCEL);
		if (isok == IDOK)
		{
			for (int d = TempDelt; d < ItemNum - 1; d++)
			{
				ItemArry[d] = ItemArry[d + 1];
			}
			ItemNum--;
			//���µ��ļ���
			SaveItem();
		}
		else if (isok == IDCANCEL)
		{
			//ȡ��ɾ����ִ���κβ���
		}
	}
}

//������Ʒ
void SearchItem()
{
	//�����û�����
	char ReceInput[64];
	TCHAR InputWindowFI[] = _T("������Ҫ���ҵ���Ʒ���ƻ�����");
	InputBox(ReceInput, 20, InputWindowFI, NULL, NULL, 0, 0, false);

	int tot=0;    //����ҵ��ķ�����������Ʒ��
	for (int w = 0; w < ItemNum; w++)
	{
		if (strcmp(ItemArry[w].i_Name, ReceInput) == 0
			|| strcmp(ItemArry[w].i_Type, ReceInput) == 0)
		{
			ItemFound[tot] = ItemArry[w];
			tot++;
		}
	}
	
	if (tot==0)    //û�ҵ�
	{
		HWND hndtipsF = GetHWnd();
		int isok = MessageBox(hndtipsF, "��������������Ʒ!", "��ʾ", MB_OK);
	}
	else
	{
		cleardevice();
		ItemPrint(tot);
	}
}

//��ʾ��ǰ�û���ӵ���Ʒ
int ShowUserItem()
{
	int tot = 0;    
	for (int w = 0; w < ItemNum; w++)
	{
		if (strcmp(ItemArry[w].i_Tel, CurrentUser.UserName) == 0)
		{
			ItemFound[tot] = ItemArry[w];
			tot++;
		}
	}

	if (tot == 0)    //��ǰ�û�û����ӹ���Ʒ
	{
		return -1;
	}
	else
	{
		for (int q = 1; q <= tot; q++)
		{
			//˳��
			settextstyle(30, 0, "����");
			char Nums[5];
			sprintf(Nums, "%d", q);

			//��������
			settextstyle(15, 0, "����");
			//����
			outtextxy(80, 40 + 40 * q, ItemFound[q - 1].i_Name);
			//����
			outtextxy(200, 40 + 40 * q, ItemFound[q - 1].i_Type);
			//˵��
			outtextxy(300, 40 + 40 * q, ItemFound[q - 1].i_Note);
			//��ϵ���ֻ�
			outtextxy(400, 40 + 40 * q, ItemFound[q - 1].i_Tel);
			//��ϵ�˵�ַ
			outtextxy(500, 40 + 40 * q, ItemFound[q - 1].i_Addr);
			//��ϵ������
			outtextxy(600, 40 + 40 * q, ItemFound[q - 1].i_Email);
		}
	}
}

//����Type.txt�ļ�
void SaveType()
{
	remove("TypeList.txt");
	//�����Ϊ�ղ�д���ļ�
	FILE* fp = fopen("TypeList.txt", "a");
	if (fp == NULL)
	{
		return;
	}
	for (int i = 0; i < TypeNum; i++)
	{
		if (strlen(TypeArry[i].t_Name) != 0)
		{
			fprintf(fp, "%s\t%s\n",
				TypeArry[i].t_Name,
				TypeArry[i].t_Attrib);
		}
	}
	fclose(fp);
}

//����������ӵ�һ�����͵�Type.txt
void SaveLastType()
{
	//��Ϊ�ղ�д���ļ�
	if (strlen(TypeArry[TypeNum].t_Name) != 0)
	{
		FILE* fp = fopen("TypeList.txt", "a");
		if (fp == NULL)
		{
			return;
		}
		fprintf(fp, "%s\t%s\n",
			TypeArry[TypeNum].t_Name,
			TypeArry[TypeNum].t_Attrib);
		fclose(fp);
		//���������������++
		TypeNum++;
	}
}

//����ͨ����˵����û�
void SaveNewCommonUser(int x)
{
	if (strlen(Regis[x].UserName) != 0)
	{
		FILE* fp = fopen("CommonUserList.txt", "a");
		if (fp == NULL)
		{
			return;
		}
		fprintf(fp, "%s\t%s\t%s\t%s\n",
			Regis[x].UserName,
			Regis[x].PassWord,
			Regis[x].Addr,
			Regis[x].Email);
		fclose(fp);
		//���������������++
		CommonUserNum++;
	}
	
}

//����Ҫɾ�����͵��±�
int DeleteTypePosition()
{
	//�����û�����
	char ReceInput[64];
	TCHAR InputWindowFI[] = _T("������Ҫɾ������������");
	InputBox(ReceInput, 20, InputWindowFI);

	//����ѭ�����ң�һ����ͬ������ѭ��
	for (int w = 0; w < TypeNum; w++)
	{
		if (strcmp(TypeArry[w].t_Name, ReceInput) == 0)
		{
			return w;
		}
	}
	//û�ҵ��ͷ���-1
	return -1;
}

//�������
void AddType()
{	
	while (true)
	{
		TCHAR InputWindow0[] = _T("��������������");
		InputBox(TypeArry[TypeNum].t_Name, 16, InputWindow0);
		//�ж��Ƿ������ظ�
		bool Flag = false;
		for (int i = 0; i < TypeNum; i++)
		{
			if (strcmp(TypeArry[TypeNum].t_Name, TypeArry[i].t_Name)==0)
			{
				Flag = true;
				break;
			}
		}
		if (Flag)
		{
			HWND hwndInput1 = GetHWnd();
			int isok = MessageBox(hwndInput1, "�������Ѵ��ڣ�����������", "��ʾ", MB_OK);
		}
		else
		{
			break;
		}
	}

	//��������˵��
	TCHAR InputWindow2[] = _T("����������͵�һ�����ԣ��硰�����ڡ�");
	InputBox(TypeArry[TypeNum].t_Attrib, 16, InputWindow2, NULL, NULL, 0, 0, false);

	//��������
	SaveLastType();
}

//ɾ������
void DeleteType()
{
	int TempDelt = DeleteTypePosition();
	if (TempDelt == -1)    //û�ҵ�
	{
		//��ȡ���ھ��
		HWND hndtipsD = GetHWnd();
		int isok = MessageBox(hndtipsD, "�����Ͳ�����!", "��ʾ", MB_OK);
	}
	else    //�ҵ��ˣ�ȷ���Ƿ�ɾ��
	{
		HWND hndtipsDS = GetHWnd();
		int isok = MessageBox(hndtipsDS, "ȷ��ɾ����", "��ʾ", MB_OKCANCEL);
		if (isok == IDOK)
		{
			for (int d = TempDelt; d < TypeNum - 1; d++)
			{
				TypeArry[d] = TypeArry[d + 1];
			}
			TypeNum--;
			//���µ��ļ���
			SaveType();
		}
		else if (isok == IDCANCEL)
		{
			//ȡ��ɾ����ִ���κβ���
		}
	}
}

//��������ע������
void ApplicationPrint()
{
	MOUSEMSG SA;
	settextcolor(BLACK);
	settextstyle(40, 0, "����");
	//���Ϊ�վͲ�����ִ��
	if (RegisNum == 0)
	{
		outtextxy(200, 200, "�������û�ע������!");
		return;
	}
	for (int q = 1; q <= RegisNum; q++)
	{
		//˳��
		char Nums[5];
		sprintf(Nums, "%d", q);

		//��������
		settextstyle(25, 0, "����");
		outtextxy(80, 40 + 40 * q, Regis[q - 1].UserName);
		outtextxy(300, 40 + 40 * q, Regis[q - 1].Addr);
		outtextxy(600, 40 + 40 * q, Regis[q - 1].Email);
	
		while (true)
		{
			SA = GetMouseMsg();
			if (SA.x >= 690 && SA.x <= 850 && SA.y >= 460 && SA.y <= 510)
			{
				//��׼����CommonUserList.txt������¼�¼
				setlinecolor(RED);
				rectangle(690, 460, 850, 510);
				if (SA.uMsg == WM_LBUTTONDOWN)
				{
					SaveNewCommonUser(q - 1);
					break;
				}
			}
			else if (SA.x >= 690 && SA.x <= 850 && SA.y >= 525 && SA.y <= 575)
			{
				//����׼������
				setlinecolor(RED);
				rectangle(690, 525, 850, 575);
				if (SA.uMsg == WM_LBUTTONDOWN)
				{
					break;
				}
			}
			else
			{
				setlinecolor(WHITE);
				rectangle(690, 460, 850, 510);
				rectangle(690, 525, 850, 575);
				rectangle(690, 590, 850, 640);
			}
		}
	}
	outtextxy(80, 80 + 40 * RegisNum, "���޸���ע�����룬��������");
	RegisNum = 0;
	remove("ApplicationList.txt");
}

//�������û�ע������
void ShowApplication()
{
	MOUSEMSG SA;
	initgraph(860, 760);
	cleardevice();
	IMAGE img;
	loadimage(&img, "./background.png", 860, 760);
	putimage(0, 0, &img);
	setfillcolor(RGB(193, 193, 193));
	setbkmode(TRANSPARENT);

	fillrectangle(690, 460, 850, 510);
	fillrectangle(690, 525, 850, 575);
	fillrectangle(690, 590, 850, 640);
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	settextstyle(20, 0, "����");
	outtextxy(750, 475, "��׼");
	outtextxy(750, 540, "����");
	outtextxy(750, 605, "����");

	ApplicationPrint();

	while (true)
	{
		SA = GetMouseMsg();
		if (SA.x >= 690 && SA.x <= 850 && SA.y >= 590 && SA.y <= 640)
		{
			//����
			setlinecolor(RED);
			rectangle(690, 590, 850, 640);
			if (SA.uMsg == WM_LBUTTONDOWN)
			{
				cleardevice();
				AdminPage();
			}
		}
		else
		{
			setlinecolor(WHITE);
			rectangle(690, 460, 850, 510);
			rectangle(690, 525, 850, 575);
			rectangle(690, 590, 850, 640);
		}
	}
}

//����Ա������ɾ����
void ManageType()
{
	MOUSEMSG SL;
	initgraph(860, 760);
	cleardevice();
	IMAGE img;
	loadimage(&img, "./background.png", 860, 760);
	putimage(0, 0, &img);
	setbkmode(TRANSPARENT);

	setfillcolor(RGB(193, 193, 193));
	fillrectangle(690, 90, 850, 140);    //��
	fillrectangle(690, 190, 850, 240);    //ɾ��
	fillrectangle(690, 590, 850, 640);    //����

	//��ͷ���ֲ���
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	settextstyle(25, 0, "����");
	outtextxy(740, 0, "ѡ��");
	outtextxy(740, 100, "���");
	outtextxy(740, 200, "ɾ��");
	outtextxy(740, 600, "����");

	TypePrint();

	//��������������
	while (true)
	{
		SL = GetMouseMsg();
		if (SL.x >= 690 && SL.x <= 850 && SL.y >= 90 && SL.y <= 140)
		{
			setlinecolor(RED);
			rectangle(690, 90, 850, 140);
			if (SL.uMsg == WM_LBUTTONDOWN)
			{
				//�����Ʒ
				AddType();
				cleardevice();
				ManageType();
			}
		}
		else if (SL.x >= 690 && SL.x <= 850 && SL.y >= 190 && SL.y <= 240)
		{
			setlinecolor(RED);
			rectangle(690, 190, 850, 240);
			if (SL.uMsg == WM_LBUTTONDOWN)
			{
				//ɾ����Ʒ
				DeleteType();
				ManageType();
			}
		}
		else if (SL.x >= 690 && SL.x <= 850 && SL.y >= 590 && SL.y <= 640)
		{
			//����
			setlinecolor(RED);
			rectangle(690, 590, 850, 640);
			if (SL.uMsg == WM_LBUTTONDOWN)
			{
				cleardevice();
				//��ɲ����������롪��ˢ��
				AdminPage();
			}
		}
		//��겻��ͣ
		else
		{
			setlinecolor(WHITE);
			rectangle(690, 90, 850, 140);    //��
			rectangle(690, 190, 850, 240);    //ɾ
			rectangle(690, 590, 850, 640);    //����
		}
	}
}

//��ͨ�û�������ɾ������Ʒ
void ManageItem()
{
	MOUSEMSG SL;
	initgraph(860, 760);
	cleardevice();
	IMAGE img;
	loadimage(&img, "./background.png", 860, 760);
	putimage(0, 0, &img);
	setbkmode(TRANSPARENT);

	setfillcolor(RGB(193,193,193));
	fillrectangle(690, 90, 850, 140);    //��
	fillrectangle(690, 190, 850, 240);    //ɾ
	fillrectangle(690, 290, 850, 340);    //��
	fillrectangle(690, 590, 850, 640);    //����

	//��ͷ���ֲ���
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	settextstyle(30, 0, "����");
	outtextxy(740, 0, "ѡ��");
	outtextxy(740, 100, "���");
	outtextxy(740, 200, "ɾ��");
	outtextxy(740, 300, "����");
	outtextxy(740, 600, "����");

	//��������������
	while (true)
	{
		SL = GetMouseMsg();
		if (SL.x >= 690 && SL.x <= 850 && SL.y >= 90 && SL.y <= 140)
		{
			setlinecolor(RED);
			rectangle(690, 90, 850, 140);
			if (SL.uMsg == WM_LBUTTONDOWN)
			{
				//�����Ʒ
				AddItem();
				cleardevice();
				//��ɲ����������롪��ˢ��
				ManageItem();
			}
		}
		else if (SL.x >= 690 && SL.x <= 850 && SL.y >= 190 && SL.y <= 240)
		{
			setlinecolor(RED);
			rectangle(690, 190, 850, 240);
			if (SL.uMsg == WM_LBUTTONDOWN)
			{
				//ɾ����Ʒ
				DeleteItem();
				//��ɲ����������롪��ˢ��
				ManageItem();
			}
		}
		else if (SL.x >= 690 && SL.x <= 850 && SL.y >= 290 && SL.y <= 340)
		{
			setlinecolor(RED);
			rectangle(690, 290, 850, 340);
			if (SL.uMsg == WM_LBUTTONDOWN)
			{
				FlagFPT = 1;
				//������Ʒ
				SearchItem();
				//��ɲ����������롪��ˢ��
				ManageItem();
			}
		}
		else if (SL.x >= 690 && SL.x <= 850 && SL.y >= 590 && SL.y <= 640)
		{
			//����
			setlinecolor(RED);
			rectangle(690, 590, 850, 640);
			if (SL.uMsg == WM_LBUTTONDOWN)
			{
				cleardevice();
				//��ɲ����������롪��ˢ��
				CommonUserPage();
			}
		}
		//��겻��ͣ
		else
		{
			setlinecolor(WHITE);
			rectangle(690, 90, 850, 140);    //��
			rectangle(690, 190, 850, 240);    //ɾ
			rectangle(690, 290, 850, 340);    //��
			rectangle(690, 590, 850, 640);    //����
		}
	}
}

//����Ļ�ϴ�ӡ�ҵ�����Ʒ��Ϣ
void ItemPrint(int tot)
{
	MOUSEMSG SA;
	initgraph(1000, 760);
	cleardevice();
	IMAGE img;
	loadimage(&img, "./background.png", 1000, 760);
	putimage(0, 0, &img);
	setfillcolor(RGB(193, 193, 193));
	setbkmode(TRANSPARENT);

	fillrectangle(690, 590, 850, 640);
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	settextstyle(20, 0, "����");
	outtextxy(750, 600, "����");

	for (int q = 1; q <= tot; q++)
	{
		//˳��
		settextstyle(30, 0, "����");
		char Nums[5];
		sprintf(Nums, "%d", q);

		//��������
		settextstyle(15, 0, "����");
		//����
		outtextxy(80, 40 + 40 * q, ItemFound[q - 1].i_Name);
		//����
		outtextxy(200, 40 + 40 * q, ItemFound[q - 1].i_Type);
		//˵��
		outtextxy(300, 40 + 40 * q, ItemFound[q - 1].i_Note);
		//��ϵ���ֻ�
		outtextxy(400, 40 + 40 * q, ItemFound[q - 1].i_Tel);
		//��ϵ�˵�ַ
		outtextxy(500, 40 + 40 * q, ItemFound[q - 1].i_Addr);
		//��ϵ������
		outtextxy(600, 40 + 40 * q, ItemFound[q - 1].i_Email);
	}

	while (true)
	{
		SA = GetMouseMsg();
		if (SA.x >= 690 && SA.x <= 850 && SA.y >= 590 && SA.y <= 640)
		{
			//����
			setlinecolor(RED);
			rectangle(690, 590, 850, 640);
			if (SA.uMsg == WM_LBUTTONDOWN)
			{
				cleardevice();
				ManageItem();
			}
		}
		else
		{
			setlinecolor(WHITE);
			rectangle(690, 590, 850, 640);
		}
	}
}

//����Ļ�ϴ�ӡ������Ϣ
void TypePrint()
{
	settextcolor(BLACK);
	settextstyle(30, 0, "����");
	//���Ϊ�վͲ�����ִ��
	if (TypeNum == 0)
	{
		outtextxy(200, 200, "��ǰ��¼Ϊ��!");
		return;
	}
	for (int q = 1; q <= TypeNum; q++)
	{
		//˳��
		char Nums[5];
		sprintf(Nums, "%d", q);

		//��������
		settextstyle(25, 0, "����");
		//����
		outtextxy(80, 40 + 40 * q, TypeArry[q - 1].t_Name);
		//����
		outtextxy(200, 40 + 40 * q, TypeArry[q - 1].t_Attrib);	
	}
}

//��ʾ��������
void ShowAllType()
{
	MOUSEMSG SA;
	initgraph(860, 760);
	cleardevice();
	IMAGE img;
	loadimage(&img, "./background.png", 860, 760);
	putimage(0, 0, &img);
	setfillcolor(RGB(193,193,193));
	setbkmode(TRANSPARENT);

	fillrectangle(690, 590, 850, 640);
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	settextstyle(20, 0, "����");
	outtextxy(750, 610, "����");

	TypePrint();

	while (true)
	{
		SA = GetMouseMsg();
		if (SA.x >= 690 && SA.x <= 850 && SA.y >= 590 && SA.y <= 640)
		{
			//����
			setlinecolor(RED);
			rectangle(690, 590, 850, 640);
			if (SA.uMsg == WM_LBUTTONDOWN)
			{
				cleardevice();
				AdminPage();
			}
		}
		else
		{
			setlinecolor(WHITE);
			rectangle(690, 590, 850, 640);
		}
	}
}

//����Ļ�ϴ�ӡ��Ʒ��Ϣ
void AllItemPrint()
{
	settextcolor(BLACK);
	settextstyle(30, 0, "����");
	//���Ϊ�վͲ�����ִ��
	if (ItemNum == 0)
	{
		outtextxy(200, 200, "��ǰ��¼Ϊ��!");
		return;
	}
	settextcolor(BLACK);
	for (int q = 1; q <= ItemNum; q++)
	{
		//˳��
		char Nums[5];
		sprintf(Nums, "%d", q);

		settextstyle(15, 0, "����");
		outtextxy(80, 40 + 40 * q, ItemArry[q - 1].i_Name);
		outtextxy(200, 40 + 40 * q, ItemArry[q - 1].i_Type);
		outtextxy(300, 40 + 40 * q, ItemArry[q - 1].i_Note);
		outtextxy(400, 40 + 40 * q, ItemArry[q - 1].i_Tel);
		outtextxy(500, 40 + 40 * q, ItemArry[q - 1].i_Addr);
		outtextxy(600, 40 + 40 * q, ItemArry[q - 1].i_Email);
	}
}

//��ʾ������Ʒ
void ShowAllItem()
{
	MOUSEMSG SA;
	initgraph(1000, 760);
	cleardevice();
	IMAGE img;
	loadimage(&img, "./background.png",1000, 760);
	putimage(0, 0, &img);
	setfillcolor(RGB(193, 193, 193));
	setbkmode(TRANSPARENT);

	fillrectangle(690, 590, 850, 640);
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	settextstyle(20, 0, "����");
	outtextxy(750, 600, "����");

	AllItemPrint();

	while (true)
	{
		SA = GetMouseMsg();
		if (SA.x >= 690 && SA.x <= 850 && SA.y >= 590 && SA.y <= 640)
		{
			//����
			setlinecolor(RED);
			rectangle(690, 590, 850, 640);
			if (SA.uMsg == WM_LBUTTONDOWN)
			{
				cleardevice();
				CommonUserPage();
			}
		}
		else
		{
			setlinecolor(WHITE);
			rectangle(690, 590, 850, 640);
		}
	}
}


int main()
{
	FirstPage();

	return 0;
}