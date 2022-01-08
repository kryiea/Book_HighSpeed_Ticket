#include<stdio.h>
#include<string.h>
#include<time.h>
#include<windows.h>
#include<stdlib.h>

#include"menu.h"

#pragma warning(disable:6011)
#pragma warning(disable:6387)
#pragma warning(disable:6031)
#pragma warning(disable:28182)

//������Ϣ �洢�ṹ
struct Route
{
	int id_Route;
	char Num_Train[7];//����� ���ڱ�𳵴�
	char Station_Begin[10];//���վ
	char Station_End[10];//����վ

	long int date_Begin;//����ʱ��
	int time_Begin;//����ʱ
	int second_Begin;//������

	int date_End;//��վ����
	int time_End;//��վʱ
	int second_End;//��վ��

	int Level[3];// 0 1 2 �ֱ����ص� һ�� ����
	int SumPeople; //��Ա����
	int Booked;//�ѹ�Ʊ����
	int Remain;//ʣ��Ʊ��
	int Wait;//������

	struct Route* next;
};

//�û���Ϣ�ṹ
struct user
{
	long long int Id;//���֤
	char Name[20];//����

	char Num_Train[7];//����� ���ڱ�𳵴�
	char Station_Begin[10];//���վ
	char Station_End[10];//����վ

	long int date_Begin;//����ʱ��
	int time_Begin;//����ʱ
	int second_Begin;//������

	int Level;//ϯ��
	int ID_Route;//����� ���ڱ�𳵴�

	struct user* next;
};

//ͷ�ڵ�
struct Route* head_Route;//·�� ͷ�ڵ�
struct user* head_Booked;//�ѹ�Ʊ�� ͷ�ڵ�
struct user* head_Wait;//���� ͷ�ڵ�
struct user* head_Waited;//�򲹳ɹ�

//�ļ�ָ��
FILE* fpRoute;
FILE* fpBookedPeo;
FILE* fpWaitPeo;
FILE* fpWaitedPeo;
FILE* fptemp;


////////////////////////////////////////////////////////////////////////////////////////////////��ʼ������/////////
//�ж��Ƿ��ҵ������ļ� ������·����Ϣ �Ѷ�Ʊ���� ������ ----- �ҵ�����1 ʧ�ܷ���0
void Find_Database(FILE* fpRoute, FILE* fpBookedPeo, FILE* fpWaitPeo, FILE* fpWaitedPeo);
//��ȡ Route.txt ��������
void InitList_Route(FILE* fpRoute);
//�ͷž�RoteLists
void FreeList_Route(struct Route* head);
//��ȡ BookedPeo.txt ��������
void InitList_BookedPeo(FILE* fpBookedPeo);
//��ȡ �������ļ� ��������
void InitList_WaitPeo();
//��ȡ �򲹳ɹ������ļ� ��������
void InitList_waitedPeo();
//�ͷ�User�����Ϣ����
void FreeList_User(struct user* head);
//����Route.txt 
void Update_FileRoute(struct Route* head_Route1);
//����user����ļ� �����������µ��ļ�����ͷ  �ļ�����ַ
void Update_FileUser(struct user* head, char* Filename);
//����waited �� ���� �� �ļ�
void Update_Waited(struct user* head_Waited, long long int ID);
//����Booked �� ���� �� �ļ�
void Update_Booked(struct user* head_Booked, long long int ID);
//���д��BookedPeo
void Write_Booked(struct user* head_Bookedpeo);

/////////////////////////////////////////////////////////////////////////////////////////////////////��Ҫ����///////
//�Զ��򲹹�Ʊ
void AutoBooked_ticket(long long int ID, int id_route, int level, char* name);
//���Ƿ� ���ܺ򲹵ĳ˿�
void Find_WaitPeo(int Idroute, int level);
//�Ƿ�ѡ���  �ɹ��򲹷��� 1
int Select_wait(int idroute, int level, struct Route* now);
// ��ѯ����
void Search_Route();
//����Ʊ
void Book_Ticket();
//�˶���Ʊ
void Cancel_Ticket();
//���˲�Ʊ
void Search_Inf();
//�������ѯ
void Search_wait();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int main()
{
	//�ж��ļ��Ƿ����
	Find_Database(fpRoute, fpBookedPeo, fpWaitPeo, fpWaitedPeo);

	//�˵�����
	int select = 0;
	UserMenu();

	while (1)
	{
		if (select != 9)
		{
			printf("������ָ�");
		}
		scanf_s("%d", &select);
		switch (select)
		{
		case 1:
			// 1.��ѯ��Ʊ
			Search_Route();
			break;
		case 2:
			//2.Ԥ����Ʊ
			Book_Ticket();
			break;
		case 3:
			//3.�˶���Ʊ
			Cancel_Ticket();
			break;
		case 4:
			//4.�򲹲�ѯ
			Search_wait();
			break;
		case 5:
			Search_Inf();
			break;
		case 9:
			system("CLS");
			AdminMenu(); //9.��̨����
			break;
		case 0:   //0.�˳�����
			printf("\n��ӭ�´�ʹ��\n");
			exit(0);
			break;

		default:
			printf("����������\n");
			Sleep(800);
			system("CLS");
			UserMenu();
			break;
		}
	}

	return 0;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//�ж��Ƿ��ҵ������ļ� ������·����Ϣ �Ѷ�Ʊ���� ������ ----- �ҵ�����1 ʧ�ܷ���0
void Find_Database(FILE* fpRoute, FILE* fpBookedPeo, FILE* fpWaitPeo,FILE* fpWaitedPeo)
{
	//fopen_s �ɹ�����0 ���ɹ�����������ֵ
	int a = fopen_s(&fpBookedPeo, "BookedPeo.txt", "r");
	int b = fopen_s(&fpWaitPeo, "WaitPeo.txt", "r");
	int c = fopen_s(&fpRoute, "Route.txt", "r");
	int d = fopen_s(&fpWaitedPeo, "WaitedPeo.txt", "r");

	if (a == 0 && b == 0 && c == 0 && d== 0)
	{
		printf("�������������ļ����ҵ�����������\n\n");
		fclose(fpRoute);
		fclose(fpBookedPeo);
		fclose(fpWaitPeo);
		fclose(fpWaitedPeo);
	
	}
	else//�ļ���ʧ
	{
		printf("�����ļ�δ�ҵ�\n");
		fclose(fpRoute);
		fclose(fpBookedPeo);
		fclose(fpWaitPeo);
		fclose(fpWaitedPeo);

		exit(0);
	}
}

//��ȡ Route.txt ��������
void InitList_Route(FILE* fpRoute)
{
	fopen_s(&fpRoute, "Route.txt", "r");// fpRoute ָ��Route.txt���ļ���

	if (!feof(fpRoute))
	{	//����ͷ�ڵ�
		head_Route = (struct Route*)malloc(sizeof(struct Route));
		head_Route->next = NULL;

		//������ʱ�ڵ� ����ˢ��
		struct Route* temp;
		temp = head_Route;

		char ch;


		while (!feof(fpRoute))
		{
			fscanf_s(fpRoute, "%d", &temp->id_Route);//����·��id

			fscanf_s(fpRoute, "%s", temp->Num_Train, 7);//���복�α��

			fscanf_s(fpRoute, "%s", temp->Station_Begin, 10);//�������վ

			fscanf_s(fpRoute, "%s", temp->Station_End, 10);//�����յ�վ

			fscanf_s(fpRoute, "%ld", &temp->date_Begin);//���뿪ʼ����

			fscanf_s(fpRoute, "%d", &temp->time_Begin);//���� ��ʼʱ

			fscanf_s(fpRoute, "%d", &temp->second_Begin);//���� ��ʼ��

			fscanf_s(fpRoute, "%ld", &temp->date_End);//�����յ�����

			fscanf_s(fpRoute, "%d", &temp->time_End);//���� �յ�ʱ

			fscanf_s(fpRoute, "%d", &temp->second_End);//���� �յ��

			fscanf_s(fpRoute, "%d %d %d", &temp->Level[0], &temp->Level[1], &temp->Level[2]);//���� �ص��� һ���� ������ ����

			fscanf_s(fpRoute, "%d %d %d %d", &temp->SumPeople, &temp->Booked, &temp->Remain, &temp->Wait);//���� ��Ա���� �ѹ�Ʊ���� ʣ��Ʊ�� ������

			ch = fgetc(fpRoute);//�̵� \n

			struct Route* Node = (struct Route*)malloc(sizeof(struct Route));

			Node->next = NULL;

			temp->next = Node;
			temp = Node;

		}
		temp = NULL;
		free(temp);

	}


	fclose(fpRoute);

}

//�ͷž�RoteLists
void FreeList_Route(struct Route* head)
{
	struct Route* pre, * p;
	pre = head;
	p = pre->next;

	while (p != NULL)
	{
		free(pre);
		pre = p;
		p = p->next;
	}
	free(pre);

}

//��ȡ BookedPeo.txt ��������
void InitList_BookedPeo(FILE* fpBookedPeo)
{
	fopen_s(&fpBookedPeo, "BookedPeo.txt", "r");

	if (!feof(fpBookedPeo))//�ǿ��ļ�
	{
		//����
		head_Booked = (struct user*)malloc(sizeof(struct user));
		head_Booked->next = NULL;

		struct user* temp_Booked;
		temp_Booked = head_Booked;//ֻ���ڼ�¼��ǰλ�� ͷ��ʼ

		if (temp_Booked != NULL && head_Booked != NULL)
		{
			while (!feof(fpBookedPeo))
			{
				fscanf_s(fpBookedPeo, "%lld", &temp_Booked->Id);//�����֤
				fscanf_s(fpBookedPeo, "%s", temp_Booked->Name, 20);//��ȡ����
				fscanf_s(fpBookedPeo, "%s", temp_Booked->Num_Train, 7);//��ȡ����
				fscanf_s(fpBookedPeo, "%s", temp_Booked->Station_Begin, 10);//�����վ
				fscanf_s(fpBookedPeo, "%s", temp_Booked->Station_End, 10);//���յ�վ
				fscanf_s(fpBookedPeo, "%ld", &temp_Booked->date_Begin);//������
				fscanf_s(fpBookedPeo, "%d", &temp_Booked->time_Begin);//��ʱ
				fscanf_s(fpBookedPeo, "%d", &temp_Booked->second_Begin);//����
				fscanf_s(fpBookedPeo, "%d", &temp_Booked->Level);//��ϯ��
				fscanf_s(fpBookedPeo, "%d", &temp_Booked->ID_Route);//��·��ID

				struct user* Node = (struct user*)malloc(sizeof(struct user));
				Node->next = NULL;
				temp_Booked->next = Node;
				temp_Booked = Node;
			}

		}
		temp_Booked = NULL;
		free(temp_Booked);
	}

	//�ر��ļ�
	fclose(fpBookedPeo);

}

//��ȡ �������ļ� ��������
void InitList_WaitPeo()
{
	fopen_s(&fpWaitPeo,"WaitPeo.txt","r");

	if (!feof(fpWaitPeo))//�ǿ��ļ�
	{
		head_Wait = (struct user*)malloc(sizeof(struct user));
		struct user* temp_Wait = head_Wait;


		while (!feof(fpWaitPeo))
		{
			fscanf_s(fpWaitPeo, "%lld", &temp_Wait->Id);//�����֤
			fscanf_s(fpWaitPeo, "%s", temp_Wait->Name, 20);//��ȡ����
			fscanf_s(fpWaitPeo, "%s", temp_Wait->Num_Train, 7);//��ȡ�����
			fscanf_s(fpWaitPeo, "%s", temp_Wait->Station_Begin, 10);//�����վ
			fscanf_s(fpWaitPeo, "%s", temp_Wait->Station_End, 10);//���յ�վ
			fscanf_s(fpWaitPeo, "%ld", &temp_Wait->date_Begin);//������
			fscanf_s(fpWaitPeo, "%d", &temp_Wait->time_Begin);//��ʱ
			fscanf_s(fpWaitPeo, "%d", &temp_Wait->second_Begin);//����
			fscanf_s(fpWaitPeo, "%d", &temp_Wait->Level);//��ϯ��
			fscanf_s(fpWaitPeo, "%d", &temp_Wait->ID_Route);//��·��ID

			struct user* Node = (struct user*)malloc(sizeof(struct user));
			Node->next = NULL;
			temp_Wait->next = Node;
			temp_Wait = Node;

		}
		temp_Wait = NULL;
		free(temp_Wait);
	}

	fclose(fpWaitPeo);
}

//��ȡ �򲹳ɹ������ļ� ��������
void InitList_waitedPeo()
{
	fopen_s(&fpWaitedPeo, "WaitedPeo.txt", "r");

	if (!feof(fpWaitedPeo))//�ǿ��ļ�
	{
		head_Waited = (struct user*)malloc(sizeof(struct user));
		struct user* temp_Waited_temp = head_Waited;


		while (!feof(fpWaitedPeo))
		{
			fscanf_s(fpWaitedPeo, "%lld", &temp_Waited_temp->Id);//�����֤
			fscanf_s(fpWaitedPeo, "%s", temp_Waited_temp->Name, 20);//��ȡ����
			fscanf_s(fpWaitedPeo, "%s", temp_Waited_temp->Num_Train, 7);//��ȡ�����
			fscanf_s(fpWaitedPeo, "%s", temp_Waited_temp->Station_Begin, 10);//�����վ
			fscanf_s(fpWaitedPeo, "%s", temp_Waited_temp->Station_End, 10);//���յ�վ
			fscanf_s(fpWaitedPeo, "%ld", &temp_Waited_temp->date_Begin);//������
			fscanf_s(fpWaitedPeo, "%d", &temp_Waited_temp->time_Begin);//��ʱ
			fscanf_s(fpWaitedPeo, "%d", &temp_Waited_temp->second_Begin);//����
			fscanf_s(fpWaitedPeo, "%d", &temp_Waited_temp->Level);//��ϯ��
			fscanf_s(fpWaitedPeo, "%d", &temp_Waited_temp->ID_Route);//��·��ID

			struct user* Node = (struct user*)malloc(sizeof(struct user));
			Node->next = NULL;
			temp_Waited_temp->next = Node;
			temp_Waited_temp = Node;

		}
		temp_Waited_temp = NULL;
		free(temp_Waited_temp);
	}

	fclose(fpWaitedPeo);

}

//�ͷ�User�����Ϣ����
void FreeList_User(struct user* head)
{
	struct user* pre, * p;
	pre = head;
	p = pre->next;

	if (head == NULL)
	{
		return;
	}

	while (p != NULL)
	{
		free(pre);
		pre = p;
		p = p->next;
	}

	free(pre);
}

///////////////////////////////////////////////////////////////////////////������////////////////////////////////////////////

//����Route.txt 
void Update_FileRoute(struct Route* head_Route1)
{

	fopen_s(&fptemp, "temp.txt", "w"); // �������ļ�
	struct Route* temp = head_Route1;

	while (temp->next->next != NULL)
	{
		fprintf_s(fptemp, "%d ", temp->id_Route);
		fprintf_s(fptemp, "%s ", temp->Num_Train);
		fprintf_s(fptemp, "%s ", temp->Station_Begin);
		fprintf_s(fptemp, "%s ", temp->Station_End);
		fprintf_s(fptemp, "%ld ", temp->date_Begin);
		fprintf_s(fptemp, "%2d ", temp->time_Begin);
		fprintf_s(fptemp, "%2d ", temp->second_Begin);
		fprintf_s(fptemp, "%ld ", temp->date_End);
		fprintf_s(fptemp, "%2d ", temp->time_End);
		fprintf_s(fptemp, "%2d ", temp->second_End);
		fprintf_s(fptemp, "%2d ", temp->Level[0]);
		fprintf_s(fptemp, "%2d ", temp->Level[1]);
		fprintf_s(fptemp, "%2d ", temp->Level[2]);
		fprintf_s(fptemp, "%2d ", temp->SumPeople);
		fprintf_s(fptemp, "%2d ", temp->Booked);
		fprintf_s(fptemp, "%2d ", temp->Remain);
		fprintf_s(fptemp, "%2d", temp->Wait);
		fprintf_s(fptemp, "\n");
		temp = temp->next;
	}

	fclose(fptemp);
	remove("Route.txt");
	rename("temp.txt", "Route.txt");


}

//����user����ļ� �����������µ��ļ�����ͷ  �ļ�����ַ
void Update_FileUser(struct user* head ,char* Filename)
{
	fopen_s(&fptemp, "temp.txt", "w"); // �������ļ�
	struct user* temp = head;


	while (temp->next->next != NULL)
	{
		fprintf_s(fptemp, "\n");
		fprintf_s(fptemp, "%lld ", temp->Id);
		fprintf_s(fptemp, "%s ", temp->Name);
		fprintf_s(fptemp, "%s ", temp->Num_Train);
		fprintf_s(fptemp, "%s ", temp->Station_Begin);
		fprintf_s(fptemp, "%s ", temp->Station_End);
		fprintf_s(fptemp, "%d ", temp->date_Begin);
		fprintf_s(fptemp, "%d ", temp->time_Begin);
		fprintf_s(fptemp, "%d ", temp->second_Begin);
		fprintf_s(fptemp, "%d ", temp->Level);
		fprintf_s(fptemp, "%d", temp->ID_Route);
		temp = temp->next;
	}

	fclose(fptemp);
	remove(Filename);
	rename("temp.txt", Filename);
}

//����waited �� ���� �� �ļ�
void Update_Waited(struct user* head_Waited, long long int ID)
{
	struct user* temp_Waited = head_Waited;//��ǰ
	struct user* temp_front = head_Waited;//��һ��

	if (temp_Waited == NULL)
	{
		return;
	}

	//�� ɾ ��ͬʱ��¼�����ڵ�
	while (temp_Waited->next != NULL)
	{

		if (ID == temp_Waited->Id) // �ҵ��ýڵ�
		{

			if (temp_Waited == head_Waited)//ɾ��ͷ�ڵ�
			{
				//��һ��Ϊ�������ݣ�����ɾ�������Ժ��Դ����
			}
			else//ɾ�� ��ͷ�ڵ�
			{
				temp_front->next = temp_Waited->next;
				free(temp_Waited);
			}
			break;
		}
		temp_front = temp_Waited;
		temp_Waited = temp_Waited->next;
	}
	//ɾ�����һ���ڵ�
	if (ID == temp_Waited->Id)
	{
		temp_front->next = NULL;
		free(temp_Waited);//�ͷ�
	}

	//���µ� List д�����WaitedPeo.txt
	Update_FileUser(head_Waited, "WaitedPeo.txt");


}

//����Booked �� ���� �� �ļ�
void Update_Booked(struct user* head_Booked, long long int ID)
{
	struct user* temp_Booked = head_Booked;//��ǰ
	struct user* temp_front = head_Booked;//��һ��

	if (temp_Booked == NULL)
	{
		return;
	}

	//�� ɾ ��ͬʱ��¼�����ڵ�
	while (temp_Booked->next != NULL)
	{

		if (ID == temp_Booked->Id) // �ҵ��ýڵ�
		{

			if (temp_Booked == head_Waited)//ɾ��ͷ�ڵ�
			{
				//��һ��Ϊ�������ݣ�����ɾ�������Ժ��Դ����
			}
			else//ɾ�� ��ͷ�ڵ�
			{
				temp_front->next = temp_Booked->next;
				free(temp_Booked);
			}
			break;
		}
		temp_front = temp_Booked;
		temp_Booked = temp_Booked->next;
	}
	//ɾ�����һ���ڵ�
	if (ID == temp_Booked->Id)
	{
		temp_front->next = NULL;
		free(temp_Booked);//�ͷ�
	}

	//�����ļ�
	Update_FileUser(head_Booked, "BookedPeo.txt");

}

//���д��BookedPeo
void Write_Booked(struct user* head_Bookedpeo)
{
	fopen_s(&fpBookedPeo, "BookedPeo.txt", "a");//ָ���ļ�β 

	fprintf_s(fpBookedPeo, "\n");

	fprintf_s(fpBookedPeo, "%lld", head_Booked->Id);//��Ʊ�����֤����
	fprintf_s(fpBookedPeo, " ");

	fputs(head_Booked->Name, fpBookedPeo);//��Ʊ������
	fprintf_s(fpBookedPeo, " ");

	fputs(head_Booked->Num_Train,fpBookedPeo);//������
	fprintf_s(fpBookedPeo, " ");

	fputs(head_Booked->Station_Begin, fpBookedPeo);//���վ
	fprintf_s(fpBookedPeo, " ");

	fputs(head_Booked->Station_End, fpBookedPeo);//�յ�վ
	fprintf_s(fpBookedPeo, " ");

	fprintf_s(fpBookedPeo, "%ld", head_Booked->date_Begin);//����
	fprintf_s(fpBookedPeo, " ");

	fprintf_s(fpBookedPeo, "%d", head_Booked->time_Begin);//ʱ
	fprintf_s(fpBookedPeo, " ");

	fprintf_s(fpBookedPeo, "%d", head_Booked->second_Begin);//��
	fprintf_s(fpBookedPeo, " ");

	fprintf_s(fpBookedPeo, "%d", head_Booked->Level);//ϯ��
	fprintf_s(fpBookedPeo, " ");
	fprintf_s(fpBookedPeo, "%d", head_Booked->ID_Route);//��id

	//�ر�
	fclose(fpBookedPeo);

	free(head_Booked);//�ͷ�


}

//�Զ��򲹹�Ʊ
void AutoBooked_ticket(long long int ID, int id_route, int level, char* name)
{
	struct Route* temp = head_Route;
	//temp �ҵ�ָ��·��
	while (temp != NULL)
	{
		if (id_route = temp->id_Route )//�ҵ�
		{
			break;
		}
		temp = temp->next;
	}

	// ��Ʊ�˽ڵ�  ��Ϣ����
	free(head_Booked);
	head_Booked = (struct user*)malloc(sizeof(struct user));
	head_Booked->Level = level;//ϯ��
	strcpy_s(head_Booked->Name, 20 ,name);//����
	head_Booked->Id = ID;//���֤��
	head_Booked->ID_Route = id_route;//·��ID
	strcpy_s(head_Booked->Station_Begin, 10, temp->Station_Begin);//���վ	
	strcpy_s(head_Booked->Station_End, 10, temp->Station_End);//�յ�վ
	head_Booked->date_Begin = temp->date_Begin;//����
	head_Booked->time_Begin = temp->time_Begin;//ʱ
	head_Booked->second_Begin = temp->second_Begin;//��
	strcpy_s(head_Booked->Num_Train, 7, temp->Num_Train);//���վ	

	//ˢ��routelist
	if (level == 0)
	{
		temp->Level[0]--;//�ص�����һ
		temp->Remain--;
		temp->Booked++;
	}
	else if (level == 1)
	{
		temp->Level[1]--;//�ص�����һ
		temp->Remain--;
		temp->Booked++;
	}
	else if (level == 2)
	{
		temp->Level[2]--;//�ص�����һ
		temp->Remain--;
		temp->Booked++;
	}
	temp->Wait--;

	//���� Route.text �ļ�  ����
	Update_FileRoute(head_Route);

	//д�� BookedPeo.txt
	Write_Booked(head_Booked);

}

//���Ƿ� ���ܺ򲹵ĳ˿�
void Find_WaitPeo( int Idroute,int level)
{
	InitList_WaitPeo();
	struct user* temp_FindWait = head_Wait;
	_Bool findwait = FALSE;

	if (temp_FindWait == NULL)
	{
		return;
	}

	while (temp_FindWait->next != NULL)
	{
		if (Idroute == temp_FindWait->ID_Route && level == temp_FindWait->Level)//���Ϻ�����:·�� + ϯ��
		{
			findwait = TRUE;

			//�Զ�����Ʊ
			AutoBooked_ticket(temp_FindWait->Id,temp_FindWait->ID_Route,temp_FindWait->Level,temp_FindWait->Name);
			printf("\nϵͳ�Ѿ�Ϊ������%s ���û��ɹ���һ�ų�Ʊ\n", temp_FindWait->Name);
			Line();



			//*** Waitpeo����
			InitList_WaitPeo();
			//ɾȥ ��д��
			struct user* temp_del_waitpeo = head_Wait;
			struct user* temp_front = head_Wait;
			_Bool delete = FALSE;

			while (temp_del_waitpeo->next != NULL)
			{

				//�е�һ���ǲ������ݣ����Բ�����ͷ�ڵ�
				if (temp_del_waitpeo->Id == temp_FindWait->Id && temp_del_waitpeo->ID_Route == temp_FindWait->ID_Route)//�ҵ������
				{

					delete = TRUE;		

					temp_front->next = temp_del_waitpeo->next;
					free(temp_del_waitpeo);
					break;
				}

				temp_front = temp_del_waitpeo;
				temp_del_waitpeo = temp_del_waitpeo->next;
			}

			//���һ���ڵ�
			if (delete == FALSE)
			{
				if (temp_del_waitpeo->Id == temp_FindWait->Id && temp_del_waitpeo->ID_Route == temp_FindWait->ID_Route)//�ҵ������
				{

					_Bool delete = TRUE;
					temp_front->next = NULL;
					free(temp_del_waitpeo);

				}
			}

			//д���ļ�
			Update_FileUser(head_Wait,"WaitPeo.txt");


			//WaitedPeo���� ������
			fopen_s(&fpWaitedPeo,"WaitedPeo.txt","a");
			fprintf_s(fpWaitedPeo, "\n");//����
			fprintf_s(fpWaitedPeo,"%lld ",temp_FindWait->Id);//���֤��
			fprintf_s(fpWaitedPeo, "%s ", temp_FindWait->Name);//����
			fprintf_s(fpWaitedPeo, "%s ", temp_FindWait->Num_Train);//�����
			fprintf_s(fpWaitedPeo, "%s ", temp_FindWait->Station_Begin);//���վ
			fprintf_s(fpWaitedPeo, "%s ", temp_FindWait->Station_End);//�յ�վ
			fprintf_s(fpWaitedPeo, "%ld ", temp_FindWait->date_Begin);//����
			fprintf_s(fpWaitedPeo, "%d ", temp_FindWait->time_Begin);//ʱ
			fprintf_s(fpWaitedPeo, "%d ", temp_FindWait->second_Begin);//��
			fprintf_s(fpWaitedPeo, "%d ", temp_FindWait->Level);//ϯ��
			fprintf_s(fpWaitedPeo, "%d", temp_FindWait->ID_Route);//ID-Route
			fclose(fpWaitedPeo);

			return;
		}
		temp_FindWait = temp_FindWait->next;
	}


	if (findwait == FALSE)//���һ���ڵ�ǰ ��δ�ҵ�
	{
		if (Idroute == temp_FindWait->ID_Route && level == temp_FindWait->Level)//���Ϻ�����
		{
			findwait = TRUE;
			//�Զ���
			AutoBooked_ticket(temp_FindWait->Id, temp_FindWait->ID_Route, temp_FindWait->Level, temp_FindWait->Name);
			printf("/n��һλ���򲹳˿��Ѿ��ɹ���\n����Ϊ��%s", temp_FindWait->Name);
			
		}
	}


	//�Ҳ������ʵ�
	if (findwait == FALSE)
	{
		printf("����Ҫ�Զ��򲹵ĳ�Ա\n");
		Line();

		return;
	}

}

//�Ƿ�ѡ���  �ɹ��򲹷��� 1
int Select_wait( int idroute , int level, struct Route* now)
{
	long long ID;
	char name[20];
	Line();
	printf("��ѡ���˺�\n������������֤���룺");
	scanf_s("%lld",&ID);

	//���ж��Ƿ��Ѿ�����ó��εĳ�Ʊ
	InitList_BookedPeo(fpBookedPeo);
	struct user* temp_if_again = head_Booked;
	long long int tempid = ID;
	while (temp_if_again != NULL)
	{
		if (temp_if_again->Id == tempid)
		{
			Line();
			printf("ÿ��֤�������ֻ�ܺ�һ��ͬһ���εĳ�Ʊ�������ظ���Ӻ򲹣�");
			Line();
			return;
			free(head_Booked);
		}
		temp_if_again = temp_if_again->next;
	}
	free(head_Booked);

	printf("\n������������");
	scanf_s("%s",name,20);



	//д��waitpeo.txt
	fopen_s(&fpWaitPeo,"WaitPeo.txt","a");
	fprintf_s(fpWaitPeo, "\n");//����
	fprintf_s(fpWaitPeo, "%lld ", ID);//���֤��
	fprintf_s(fpWaitPeo, "%s ", name);//����
	fprintf_s(fpWaitPeo, "%s ", now->Num_Train);//�����
	fprintf_s(fpWaitPeo, "%s ", now->Station_Begin);//���վ
	fprintf_s(fpWaitPeo, "%s ", now->Station_End);//�յ�վ
	fprintf_s(fpWaitPeo, "%ld ", now->date_Begin);//����
	fprintf_s(fpWaitPeo, "%d ", now->time_Begin);//ʱ
	fprintf_s(fpWaitPeo, "%d ", now->second_Begin);//��
	fprintf_s(fpWaitPeo, "%d ", level);//ϯ��
	fprintf_s(fpWaitPeo, "%d", idroute);//ID-Route
	fclose(fpWaitPeo);

	printf("��Ӻ򲹳ɹ���\n");
	Line();

	return 1;
}

// ��ѯ����
void Search_Route() 
{
	//·�߼���
	InitList_Route(fpRoute);

	struct Route* temp = head_Route;
	char search_End[10];
	char search_Begin[10];
	int count = 0;
	Line();
	char ch = getchar();
	printf("�������վ��");
	gets_s(search_Begin, 10);
	printf("�����յ�վ��");
	gets_s(search_End,10);

	printf("\nID\t����\t���վ\t����վ\t����ʱ��        ��վʱ��\t�ص���\tһ����\t������\t��Ʊ\t�ѹ�Ʊ\tʣ��Ʊ\t����\n");

	while (temp->next != NULL)
	{

		if ( strcmp(temp->Station_End, search_End) == 0  && strcmp(temp->Station_Begin, search_Begin) == 0)
		{
			count++;
			printf("%d\t%s   %s  %s  %ld-%d:%d  %ld-%d:%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
					temp->id_Route,
					temp->Num_Train,
					temp->Station_Begin,
					temp->Station_End,
					temp->date_Begin,
					temp->time_Begin,
					temp->second_Begin,
					temp->date_End,
					temp->time_End,
					temp->second_End,
					temp->Level[0],
					temp->Level[1],
					temp->Level[2],
					temp->SumPeople,
					temp->Booked,
					temp->Remain,
					temp->Wait

					);

			//��¼��ѯ����ĵ�ַ 

		}
		temp = temp->next;
	}

	if (strcmp(temp->Station_End, search_End) == 0 && strcmp(temp->Station_Begin, search_Begin) == 0)
	{
		count++;
		printf("%d\t%s  %s  %s  %ld-%d:%d  %ld-%d:%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
			temp->id_Route,
			temp->Num_Train,
			temp->Station_Begin,
			temp->Station_End,
			temp->date_Begin,
			temp->time_Begin,
			temp->second_Begin,
			temp->date_End,
			temp->time_End,
			temp->second_End,
			temp->Level[0],
			temp->Level[1],
			temp->Level[2],
			temp->SumPeople,
			temp->Booked,
			temp->Remain,
			temp->Wait
		);

	}	
	printf("\n");
	if (count ==  0)
	{
		printf("\n^^^^^��ǰû�к��ʵĳ���^^^^^^\n\n");
	}
	else
	{
		printf("\n�ҵ�%d������\n",count);
	}
	Line();
}

//����Ʊ
void Book_Ticket()
{
	InitList_Route(fpRoute);

	int flag = 0;
	struct Route* temp = head_Route;
	_Bool find  = FALSE;

start:
	printf("\n�����������ε� ID ��");
	scanf_s("%d",&flag);
	
	//�Ҹ�ID�Ƿ���ȷ
	while (temp->next != NULL)
	{
		if (flag == temp->id_Route)
		{
			find = TRUE;
			printf("\nID\t����\t���վ\t����վ\t����ʱ��        ��վʱ��\t�ص���\tһ����\t������\t��Ʊ\t�ѹ�Ʊ\tʣ��Ʊ\t����\n");
			printf("%d\t%s   %s  %s  %ld-%d:%d  %ld-%d:%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
				temp->id_Route,
				temp->Num_Train,
				temp->Station_Begin,
				temp->Station_End,
				temp->date_Begin,
				temp->time_Begin,
				temp->second_Begin,
				temp->date_End,
				temp->time_End,
				temp->second_End,
				temp->Level[0],
				temp->Level[1],
				temp->Level[2],
				temp->SumPeople,
				temp->Booked,
				temp->Remain,
				temp->Wait
			);
			break;
		}		
		
		temp = temp->next;
	}
	if (flag == temp->id_Route && find ==  FALSE)
	{
		find = TRUE;
		printf("\nID\t����\t���վ\t����վ\t����ʱ��        ��վʱ��\t�ص���\tһ����\t������\t��Ʊ\t�ѹ�Ʊ\tʣ��Ʊ\t����\n");
		printf("%d\t%s   %s  %s  %ld-%d:%d  %ld-%d:%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
			temp->id_Route,
			temp->Num_Train,
			temp->Station_Begin,
			temp->Station_End,
			temp->date_Begin,
			temp->time_Begin,
			temp->second_Begin,
			temp->date_End,
			temp->time_End,
			temp->second_End,
			temp->Level[0],
			temp->Level[1],
			temp->Level[2],
			temp->SumPeople,
			temp->Booked,
			temp->Remain,
			temp->Wait
		);
	}

	if (find)//���ID����
	{
		Line();
		printf("\nȷ����Ϣ��������������: 1\n������������������: 0  \n�����룺");
		int a;
		scanf_s("%d",&a);

start1:
		if (a == 1)
		{
			Line();
			//�ж� ��λ�Ƿ��㹻
			int temp_Level;
			printf("\n�ص��� һ���� ������\n�ֱ�����: 0 1 2\n");
			printf("����ѡ��:");
			scanf_s("%d",&temp_Level);
			if (temp_Level == 0)
			{
				if (temp->Level[0] <= 0)//�ص���ûƱ
				{
					Line();
					printf("�ص���Ʊ������\nѡ�������:1\n����ѡ����ϯ����:2 \n������:");
					int select1;
					scanf_s("%d",&select1);
					if (select1 == 2)//����ѡ��
					{
						goto start1;
					}
					else if (select1 == 1)//ѡ���
					{
						int aa = Select_wait(flag,temp_Level,temp);
						if (aa == 1) { temp->Wait++; }
						//���� Route.text �ļ�  ����
						Update_FileRoute(head_Route);

						return;
					}
					else
					{ 
						goto start1;
					}

				}
				
			}
			else if (temp_Level == 1)
			{
				if (temp->Level[1] <= 0)//һ����ûƱ
				{
					Line();
					printf("һ����Ʊ������\nѡ�������:1\n����ѡ����ϯ����:2 \n������:");
					int select1;
					scanf_s("%d", &select1);
					if (select1 == 2)//����ѡ��
					{
						goto start1;
					}
					else if (select1 == 1)//ѡ���
					{
						int aa = Select_wait(flag, temp_Level, temp);
						if (aa == 1) { temp->Wait++; }
						//���� Route.text �ļ�  ����
						Update_FileRoute(head_Route);

						return;
					}
					else
					{
						goto start1;
					}
				}
				
			}
			else if (temp_Level == 2)
			{
				if (temp->Level[2] <= 0)//������ûƱ
				{
					Line();
					printf("������Ʊ������\nѡ�������:1\n����ѡ����ϯ����:2 \n������:");
					int select1;
					scanf_s("%d", &select1);
					if (select1 == 2)//����ѡ��
					{
						goto start1;
					}
					else if (select1 == 1)//ѡ���
					{
						int aa = Select_wait(flag, temp_Level, temp);
						if (aa == 1) { temp->Wait++; }
						//���� Route.text �ļ�  ����
						Update_FileRoute(head_Route);
						return;
					}
					else
					{
						goto start1;
					}
				}		
			}
			else // �������
			{
				printf("�������ָ���д����˳�\n");
				return;
			}


			//���ж��Ƿ��Ѿ�����ó��εĳ�Ʊ
			InitList_BookedPeo(fpBookedPeo);
			struct user* temp_if_again = head_Booked;
			char tempname[20];
			long long int tempid;

			printf("����������");
			char ch = getchar();
			gets_s(tempname,20);
			printf("�������֤�ţ�");
			scanf_s("%lld", &tempid);//���֤��

			while (temp_if_again != NULL)
			{
				if (temp_if_again->Id == tempid)
				{
					Line();
					printf("ÿ��֤�������ֻ�ܹ���һ��ͬһ���εĳ�Ʊ�������ظ�����");
					Line();
					return;
					free(head_Booked);
				}
				temp_if_again = temp_if_again->next;
			}

			free(head_Booked);

			// ��Ʊ�� ͷ�ڵ�  ��Ϣ����
			head_Booked = (struct user*)malloc(sizeof(struct user));
			head_Booked->Level = temp_Level;//д��ϯ��
			head_Booked->Id = tempid;//���֤��
			strcpy_s(head_Booked->Name, 20, tempname);//����
			head_Booked->ID_Route = flag;//·��ID
			strcpy_s(head_Booked->Station_Begin,10,temp->Station_Begin);//���վ	
			strcpy_s(head_Booked->Station_End,10, temp->Station_End);//�յ�վ
			head_Booked->date_Begin = temp->date_Begin ;//����
			head_Booked->time_Begin = temp->time_Begin;//ʱ
			head_Booked->second_Begin = temp->second_Begin;//��
			strcpy_s(head_Booked->Num_Train, 7, temp->Num_Train);//���վ	




			//ˢ��routelist
			if (temp_Level == 0)
			{
				temp->Level[0]--;//�ص�����һ
				temp->Remain--;
				temp->Booked++;

			}
			else if (temp_Level == 1)
			{
				temp->Level[1]--;//�ص�����һ
				temp->Remain--;
				temp->Booked++;
			}
			else if (temp_Level == 2)
			{
				temp->Level[2]--;//�ص�����һ
				temp->Remain--;
				temp->Booked++;
			}


			
			//��Ʊ�ɹ�
			Line();
			printf("��ϲ�� ��Ʊ�ɹ�^_^\n");
			printf("����ţ�%s\n����ʱ�䣺%ld-%d:%d\n", temp->Num_Train, temp->date_Begin, temp->time_Begin, temp->second_Begin);
			printf("����ǰ30mins���׼�Ʊ��^_^\n");
			Line();

			//���� Route.text �ļ�  ����
			Update_FileRoute(head_Route);

			//д�� BookedPeo.txt
			Write_Booked(head_Booked);
				
		}
		else if(a == 0)
		{
			/*Line();
			temp = head_Route;
			goto start;*/
			Line();
			return;
		}
		else
		{
			Line();
			return;
		}


	}
	else//ID����ȷ
	{
		printf("�����ID�������飡\n");
		goto start;
	}



}

//�˶���Ʊ
void Cancel_Ticket()
{	
start4:
	long long int ID;
	int id_route;

	Line();
	printf("������Ʊ����\n");
	printf("�������֤���룺");
	scanf_s("%lld", &ID);
	printf("������·��ID��");
	scanf_s("%d", &id_route);

	InitList_BookedPeo(fpBookedPeo);//����BookedList
	InitList_waitedPeo(fpWaitedPeo);//���� �򲹳ɹ�List

	//������ID
	struct user* temp_Booked = head_Booked;
	struct user* temp_Waited = head_Waited;

	//��¼�˶�Ʊ����Ϣ
	long long int cancel_id_route = id_route;
	int cancel_level;

	//����Ƿ��ҵ�
	_Bool find_Booked = FALSE;
	_Bool find_Waited = FALSE;

	//���Ƿ��Ѿ���Ʊ �������������س�Ʊ��Ϣ
	while (temp_Booked->next != NULL)
	{
		if (ID == temp_Booked->Id && id_route == temp_Booked->ID_Route)//�ҵ�
		{	
			find_Booked = TRUE;
			cancel_level = temp_Booked->Level;

			//����Ƿ�Ϊ�Զ��򲹵��û�
			while (temp_Waited->next != NULL)
			{
				if (ID == temp_Waited->Id && id_route == temp_Waited->ID_Route)//ȷ���� �Զ��� ���û�
				{
					find_Waited = TRUE;
					break;
				}
				temp_Waited = temp_Waited->next;
			}
		}
		temp_Booked = temp_Booked->next;
	}
	//���һ���ڵ�֮ ǰ ��δ�ҵ�
	if (find_Booked == FALSE)
	{
		if (ID == temp_Booked->Id && id_route == temp_Booked->ID_Route)//�ҵ�
		{
			find_Booked = TRUE;
			cancel_level = temp_Booked->Level;

			//����Ƿ�Ϊ�Զ��򲹵��û�
			while (temp_Waited->next != NULL)
			{
				if (ID == temp_Waited->Id && id_route == temp_Waited->ID_Route)//ȷ���� �Զ��� ���û�
				{
					find_Waited = TRUE;
					break;
				}
			}
		}

	}


	// �ҵ���Ʊ��¼ + ���Զ���
	if (find_Booked == TRUE && find_Waited == TRUE)
	{
		InitList_waitedPeo(fpWaitedPeo);//�����Զ��򲹳ɹ����û�
		Update_Waited(head_Waited, ID);//ˢ��WaitedList,����WaitedPeo.txt

		InitList_BookedPeo(fpBookedPeo);//����BookedpeoList
		Update_Booked(head_Booked, ID);//ˢ��BookedPeoList,����Booked.txt

		//����Route  �����ҵ�Ŀ��ڵ� �������ݺ� �����ļ�
		_Bool find_route = FALSE;
		struct Route* temp_route = head_Route;
		if (temp_route == NULL)
		{
			return;
		}

		while (temp_route->next != NULL)//�����Ҹýڵ㲢����List
		{
			if (temp_Waited->ID_Route == temp_route->id_Route)//�ҵ���Ŀ����·
			{
				find_route = TRUE;
				//����Ʊ��
				if (temp_Waited->Level == 0)//�ص���
				{
					temp_route->Level[0]++;
				}
				else if (temp_Waited->Level == 1)//һ����
				{
					temp_route->Level[1]++;
				}
				else if (temp_Waited->Level == 2)//������
				{
					temp_route->Level[2]++;
				}
				temp_route->Remain++;//��Ʊ��1
				temp_route->Booked--;//�ѹ�-1
				break;
			}
			temp_route = temp_route->next;
		}

		if (find_route == FALSE)
		{
			if (temp_Waited->ID_Route == temp_route->id_Route)//�ҵ���Ŀ����·
			{
				find_route = TRUE;
				//����Ʊ��
				if (temp_Waited->Level == 0)//�ص���
				{
					temp_route->Level[0]++;
				}
				else if (temp_Waited->Level == 1)//һ����
				{
					temp_route->Level[1]++;
				}
				else if (temp_Waited->Level == 2)//������
				{
					temp_route->Level[2]++;
				}

				temp_route->Remain++;//��Ʊ��1
				temp_route->Booked--;//�ѹ�-1
			}
		}
		Update_FileRoute(head_Route);//����route�ļ�


		//���������� �ҵ������������û� ת���Զ���Ʊ
		InitList_WaitPeo();//���ش�������
		Find_WaitPeo(id_route, cancel_level);//�������Ƿ��з���Ҫ���

	}

	//�ҵ���Ʊ��¼  +  ���Զ���
	if (find_Booked == TRUE && find_Waited == FALSE)
	{
		InitList_BookedPeo(fpBookedPeo);//����BookedpeoList
		Update_Booked(head_Booked, ID);//ˢ��BookedPeoList , ����Booked.txt


		//����Route  �����ҵ�Ŀ��ڵ� �������ݺ� �����ļ�
		InitList_Route(fpRoute);
		_Bool find_route = FALSE;
		struct Route* temp_route = head_Route;
		if (temp_route == NULL)
		{
			return;
		}
		//�����Ҹýڵ㲢����List
		while (temp_route->next != NULL)
		{
			if (temp_Waited->ID_Route == temp_route->id_Route)//�ҵ���Ŀ����·
			{
				find_route = TRUE;

				//����Ʊ��
				if (temp_Waited->Level == 0)//�ص���
				{
					temp_route->Level[0]++;
				}
				else if (temp_Waited->Level == 1)//һ����
				{
					temp_route->Level[1]++;
				}
				else if (temp_Waited->Level == 2)//������
				{
					temp_route->Level[2]++;
				}
				temp_route->Remain++;//��Ʊ��1
				temp_route->Booked--;//�ѹ�-1

				break;
			}
			temp_route = temp_route->next;
		}
		if (find_route == FALSE)
		{
			if (temp_Waited->ID_Route == temp_route->id_Route)//�ҵ���Ŀ����·
			{
				find_route = TRUE;
				//����Ʊ��
				if (temp_Waited->Level == 0)//�ص���
				{
					temp_route->Level[0]++;
				}
				else if (temp_Waited->Level == 1)//һ����
				{
					temp_route->Level[1]++;
				}
				else if (temp_Waited->Level == 2)//������
				{
					temp_route->Level[2]++;
				}

				temp_route->Remain++;//��Ʊ��1
				temp_route->Booked--;//�ѹ�-1
			}
		}

		//����route�ļ�
		Update_FileRoute(head_Route);
		Line();
		printf("���Ѿ��ɹ���Ʊ ^_^  ^_^\n");
		Line();

		//���������� �ҵ������������û� ת���Զ���Ʊ
		Find_WaitPeo(id_route, cancel_level);//�������Ƿ��з���Ҫ���

	}

	//�Ҳ�����Ʊ��¼
	if(find_Booked == FALSE && find_Waited == FALSE)
	{
			printf("�����֤����δ��Ʊ�����ѯ��Ʊ��Ϣ�����ԣ�\n���������������˵��������������֤���룺1 \n�����룺");
			int select;
			scanf_s("%d",&select);
			if (select == 1)
			{
				goto start4;
			}
			else
			{
				return;
			}

	}
	

}

//���˲�Ʊ
void Search_Inf()
{
	_Bool find = FALSE;
	long long int wait_ID;
start2:
	printf("���Ѿ�������˳�Ʊ��ѯ����\n");
	printf("������������֤���룺");
	scanf_s("%lld", &wait_ID);
	Line();
	printf("�鵽������Ϣ��\n");

	//�������֤��
	struct user* temp_1;
	//FreeList_User(head_Booked);
	InitList_BookedPeo(fpBookedPeo);

	temp_1 = head_Booked;

	char ch[7];
	char L0[] = "�ص���";
	char L1[] = "һ����";
	char L2[] = "������";

	while (temp_1->next != NULL)
	{
		if (temp_1->Id == wait_ID)
		{
			find = TRUE;

			if (temp_1->Level == 0)
			{
				strcpy_s(ch, 7, L0);
			}
			else if (temp_1->Level == 1)
			{
				strcpy_s(ch, 7, L1);

			}
			else if (temp_1->Level == 2)
			{
				strcpy_s(ch, 7, L2);
			}

			//�����Ϣ
			printf("\n·��ID: %d ������%s ���Σ�%s ���վ��%s �յ�վ��%s ����ʱ�䣺%ld-%d:%d ϯ��%s\n",
				temp_1->ID_Route,
				temp_1->Name,
				temp_1->Num_Train,
				temp_1->Station_Begin, 
				temp_1->Station_End,
				temp_1->date_Begin, 
				temp_1->time_Begin,
				temp_1->second_Begin,
				ch
			);

		}

		temp_1 = temp_1->next;
	
	}
	if (temp_1->Id == wait_ID)//�ж����һ������
	{
		find = TRUE;
		if (temp_1->Level == 0)
		{
			strcpy_s(ch, 7, L0);
		}
		else if (temp_1->Level == 1)
		{
			strcpy_s(ch, 7, L1);

		}
		else if (temp_1->Level == 2)
		{
			strcpy_s(ch, 7, L2);
		}


		printf("\n·��ID: %d ������%s ���Σ�%s ���վ��%s �յ�վ��%s ����ʱ�䣺%ld-%d:%d ϯ��%s\n",
			temp_1->ID_Route,
			temp_1->Name,
			temp_1->Num_Train,
			temp_1->Station_Begin,
			temp_1->Station_End,
			temp_1->date_Begin,
			temp_1->time_Begin,
			temp_1->second_Begin,
			ch
		);

	}

	if (find == TRUE)
	{
		Line();
	}

	if (find == FALSE)
	{
		printf("\n��ѯ������ؼ�¼\n");
		Line();
		return;
	}

}

//�������ѯ
void Search_wait()

{
	long long int ID;
	printf("���Ѿ�����򲹽����ѯ\n");
	printf("�������֤�����ѯ��");
	scanf_s("%lld",&ID);

	//���ش��� �� �򲹳ɹ� ����
	InitList_waitedPeo();
	InitList_WaitPeo();


	//�ȼ�� �򲹳ɹ�
	struct user* temp_waited = head_Waited;
	struct user* temp_wait = head_Wait;
	_Bool findWaited = FALSE;
	_Bool findWait = FALSE;


	//��ʼ�Һ򲹳ɹ�������
	while(temp_waited->next != NULL)
	{
		if (ID ==temp_waited->Id)//�ҵ�
		{
			findWaited = TRUE;
			char ch[7];
			char L0[] = "�ص���";
			char L1[] = "һ����";
			char L2[] = "������";
			if (temp_waited->Level == 0)
			{
				strcpy_s(ch, 7, L0);
			}
			else if (temp_waited->Level == 1)
			{
				strcpy_s(ch, 7, L1);

			}
			else if (temp_waited->Level == 2)
			{
				strcpy_s(ch, 7, L2);
			}

			Line();
			printf("\n^_^  ^_^�Ѿ��򲹳ɹ�����ǰ�򲹳ɹ��ĳ�Ʊ��ϢΪ��\n");
			printf("\n·��ID��%d ������%s ����ţ�%s ϯ��%s ���վ��%s �յ�վ��%s ����ʱ�䣺%ld-%d:%d\n",
				temp_waited->ID_Route,
				temp_waited->Name,
				temp_waited->Num_Train,
				ch,
				temp_waited->Station_Begin,
				temp_waited->Station_End,
				temp_waited->date_Begin,
				temp_waited->time_Begin,
				temp_waited->second_Begin			
				);
			Line();
		}
		temp_waited = temp_waited->next;
	}
	//û�ҵ����������һ���ڵ�
	if (findWaited == FALSE)
	{
		if (ID == temp_waited->Id)//�ҵ�
		{
			findWaited = TRUE;
			char ch[7];
			char L0[] = "�ص���";
			char L1[] = "һ����";
			char L2[] = "������";
			if (temp_waited->Level == 0)
			{
				strcpy_s(ch, 7, L0);
			}
			else if (temp_waited->Level == 1)
			{
				strcpy_s(ch, 7, L1);

			}
			else if (temp_waited->Level == 2)
			{
				strcpy_s(ch, 7, L2);
			}

			Line();
			printf("\n^_^  ^_^�Ѿ��򲹳ɹ���\n��ǰ�򲹳ɹ��ĳ�Ʊ��ϢΪ��\n");
			printf("\n·��ID��%d ������%s ����ţ�%s ϯ��%s ���վ��%s �յ�վ��%s ����ʱ�䣺%ld-%d:%d\n",
				temp_waited->ID_Route,
				temp_waited->Name,
				temp_waited->Num_Train,
				ch,
				temp_waited->Station_Begin,
				temp_waited->Station_End,
				temp_waited->date_Begin,
				temp_waited->time_Begin,
				temp_waited->second_Begin
			);
			Line();
		}
	}

	//�򲹳ɹ������Ҳ��� ���ټ���������
	if (findWaited == FALSE)
	{
		//��ʼ�Ҵ��򲹵�����
		while (temp_wait->next != NULL)
		{
			if (ID == temp_wait->Id)//�ҵ�
			{
				findWait = TRUE;
				char ch[7];
				char L0[] = "�ص���";
				char L1[] = "һ����";
				char L2[] = "������";
				if (temp_wait->Level == 0)
				{
					strcpy_s(ch, 7, L0);
				}
				else if (temp_wait->Level == 1)
				{
					strcpy_s(ch, 7, L1);

				}
				else if (temp_wait->Level == 2)
				{
					strcpy_s(ch, 7, L2);
				}

				Line();
				printf("\n���ڵȴ���״̬����ǰ�ȴ��򲹳�Ʊ��ϢΪ��\n");
				printf("\n·��ID��%d ������%s ����ţ�%s ϯ��%s ���վ��%s �յ�վ��%s ����ʱ�䣺%ld-%d:%d\n",
					temp_wait->ID_Route,
					temp_wait->Name,
					temp_wait->Num_Train,
					ch,
					temp_wait->Station_Begin,
					temp_wait->Station_End,
					temp_wait->date_Begin,
					temp_wait->time_Begin,
					temp_wait->second_Begin
				);
				Line();
				break;
			}
			temp_wait = temp_wait->next;
		}
		//û�ҵ����������һ���ڵ�
		if (findWait == FALSE)
		{
			if (ID == temp_wait->Id)//�ҵ�
			{
				findWait = TRUE;

				char ch[7];
				char L0[] = "�ص���";
				char L1[] = "һ����";
				char L2[] = "������";
				if (temp_wait->Level == 0)
				{
					strcpy_s(ch, 7, L0);
				}
				else if (temp_wait->Level == 1)
				{
					strcpy_s(ch, 7, L1);

				}
				else if (temp_wait->Level == 2)
				{
					strcpy_s(ch, 7, L2);
				}

				Line();
				printf("\n·��ID��%d ������%s ����ţ�%s ϯ��%s ���վ��%s �յ�վ��%s ����ʱ�䣺%ld-%d:%d\n",
					temp_wait->ID_Route,
					temp_wait->Name,
					temp_wait->Num_Train,
					ch,
					temp_wait->Station_Begin,
					temp_wait->Station_End,
					temp_wait->date_Begin,
					temp_wait->time_Begin,
					temp_wait->second_Begin
				);
				Line();
			}
		}
	}
	
	//������������˵��û�вμӹ���
	if(findWaited == FALSE && findWait == FALSE )
	{
		Line();
		printf("\n�����֤����:%lld \n�޺���ص���Ϣ\n",ID);
		Line();
	}

}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////