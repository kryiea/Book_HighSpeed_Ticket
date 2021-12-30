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






//��һ�� �洢�ṹ
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

//��Ʊ�� �洢�ṹ
struct BookedPeo
{
	char Name[20];//����
	long long int Id;//���֤

	int ID_Train;//����� ���ڱ�𳵴�
	char Station_Begin[10];//���վ
	char Station_End[10];//����վ

	long int date_Begin;//����ʱ��
	int time_Begin;//����ʱ
	int second_Begin;//������

	int Level;//ϯ��

	struct BookedPeo* next;
};

//���� �洢�ṹ
struct WaitPeo
{
	char Name[20];//����
	long long int Id;//���֤
	int ID_Train;//Ŀ�공���

	char Station_Begin[10];//���վ
	char Station_End[10];//����վ

	long int date_Begin;//����ʱ��
	int time_Begin;//����ʱ
	int second_Begin;//������

	int Level;//ϯ��


	struct WaitPeo* next;

};



struct Route* head_Route;//·��ͷ�ڵ�
struct BookedPeo* head_Booked;//�ѹ�Ʊ��ͷ�ڵ�


//�ļ�ָ��
FILE* fpRoute;
FILE* fpBookedPeo;
FILE* fpWaitPeo;
FILE* fptemp;



//�ж��Ƿ��ҵ������ļ� ������·����Ϣ �Ѷ�Ʊ���� ������ ----- �ҵ�����1 ʧ�ܷ���0
void FindDatabase(FILE** fpRoute, FILE** fpBookedPeo, FILE** fpWaitPeo)
{
	//fopen_s �ɹ�����0 ���ɹ�����������ֵ
	int a = fopen_s(fpBookedPeo, "BookedPeo.txt", "r");
	int b = fopen_s(fpWaitPeo, "WaitPeo.txt", "r");
	int c = fopen_s(fpRoute, "Route.txt", "r");

	if (a == 0 && b == 0 && c == 0)
	{
		printf("�����ļ����ҵ�\n");
	
	}
	else//�ļ���ʧ
	{
		printf("�����ļ�δ�ҵ�\n");
		exit(0);
	}
}

//��ȡ ·����Ϣ�ļ� ��������
void InitList_Route(FILE** fpRoute_r, FILE* fpRoute)
{
	//����ͷ�ڵ�
	head_Route = (struct Route*)malloc(sizeof(struct Route));
	head_Route->next = NULL;

	//������ʱ�ڵ� ����ˢ��
	struct Route* temp;
	temp = head_Route;

	char ch;

	fopen_s(fpRoute_r, "Route.txt", "r");// fpRoute ָ��Route.txt���ļ���

	while (!feof(fpRoute))
	{
		fscanf_s(fpRoute,"%d",&temp->id_Route);//���복id
		ch = fgetc(fpRoute);//�̵� \n

		fgets(temp->Num_Train, 7, fpRoute);//���복���
		temp->Num_Train[5] = '\0';

		fgets(temp->Station_Begin, 8, fpRoute);//�������վ	
		temp->Station_Begin[6] = '\0';

		fgets(temp->Station_End, 8, fpRoute);//�����յ�վ
		temp->Station_End[6] = '\0';

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
	fclose(fpRoute);

}

//��ȡ �Ѷ�Ʊ�����ļ� ��������
void InitList_BookedPeo()
{
	fclose(fpBookedPeo);
	fopen_s(&fpBookedPeo, "BookedPeo.txt", "r");
	//����
	head_Booked = (struct BookedPeo*)malloc(sizeof(struct BookedPeo));
	head_Booked->next = NULL;
	struct BookedPeo* temp_Booked = NULL;
	temp_Booked = head_Booked;
	while (!feof(fpBookedPeo))
	{
		fscanf_s(fpBookedPeo, "%lld", &temp_Booked->Id);
		//char ch = fgetc(fpBookedPeo);
		//fscanf_s(fpBookedPeo, "%s",temp_Booked->Name);
		//fgets(temp_Booked->Name,20,fpBookedPeo);
		//strcpy_s(temp_Booked->Name,strlen(temp),temp);
		//fscanf_s(fpBookedPeo, "%d", &temp_Booked->Level);
		//fscanf_s(fpBookedPeo, "%d", &temp_Booked->ID_Train);


		struct BookedPeo* Node = (struct BookedPeo*)malloc(sizeof(struct BookedPeo));
		Node->next = NULL;
		temp_Booked->next = Node;
		temp_Booked = Node;

	}

	//�ر��ļ�
	fclose(fpBookedPeo);

}

//��ȡ �������ļ� ��������
void InitList_WaitPeo()
{

}

// ��ѯ����
void Search_Route() 
{
	struct Route* temp = head_Route;
	char search[10];
	int count = 0;
	Line();
	printf("�����յ�վ��");
	char ch = getchar();
	gets_s(search,10);
	
	
	printf("\nID\t����\t���վ\t����վ\t����ʱ��        ��վʱ��\t�ص���\tһ����\t������\t��Ʊ\t�ѹ�Ʊ\tʣ��Ʊ\t����\n");

	while (temp->next != NULL)
	{

		if ( strcmp(temp->Station_End, search) == 0 )
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

	if (strcmp(temp->Station_End, search) == 0)
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
		printf("\nȷ����Ϣ������: 1  \n����ѡ��ID����: 0  \n������������������  \n");
		int a;
		scanf_s("%d",&a);

start1:
		if (a == 1)
		{
			//�ж� ��λ�Ƿ��㹻
			int temp_Level;
			printf("\n�ص��� һ���� ������ �ֱ����� 0 1 2\n");
			printf("������ҪԤ���� ϯ��:");
			scanf_s("%d",&temp_Level);
			if (temp_Level == 0)
			{
				if (temp->Level[0] <= 0)//�ص���ûƱ
				{
					printf("�ص���Ʊ�����㣬������ѡ����ϯ\n");
					goto start1;
				}
				
			}
			else if (temp_Level == 1)
			{
				if (temp->Level[1] <= 0)//һ����ûƱ
				{
					printf("һ����Ʊ�����㣬������ѡ����ϯ\n");
					goto start1;
				}
				
			}
			else if (temp_Level == 2)
			{
				if (temp->Level[2] <= 0)//������ûƱ
				{
					printf("������Ʊ�����㣬������ѡ����ϯ\n");
					goto start1;
				}		
			}
			else // �������
			{
				printf("�������ָ���д����˳�\n");
				exit(0);
			}

			//���� ��Ʊ�� ͷ�ڵ�
			head_Booked = (struct BookedPeo*)malloc(sizeof(struct BookedPeo));
			head_Booked->Level = temp_Level;//д��ϯ��
			printf("����������");
			char ch = getchar();
			gets_s(head_Booked->Name,20);
			printf("�������֤�ţ�");
			scanf_s("%lld", &head_Booked->Id);
			head_Booked->ID_Train = flag;

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


			//д�� BookedPeo.txt
			fclose(fpBookedPeo);
			fopen_s(&fpBookedPeo,"BookedPeo.txt","a");//ָ���ļ�β ��д
			fprintf_s(fpBookedPeo,"%lld",head_Booked->Id);//��Ʊ�����֤����
			fprintf_s(fpBookedPeo, " ");
			fputs(head_Booked->Name, fpBookedPeo);//��Ʊ������
			fprintf_s(fpBookedPeo, " ");
			fprintf_s(fpBookedPeo, "%d", head_Booked->Level);//ϯ��
			fprintf_s(fpBookedPeo, " ");
			fprintf_s(fpBookedPeo, "%d", head_Booked->ID_Train);//��id
			fprintf_s(fpBookedPeo, "\n");

			//��Ʊ�ɹ�
			Line();
			printf("��ϲ�� ��Ʊ�ɹ�^_^\n");
			printf("����ţ�%s\n����ʱ�䣺%ld-%d:%d\n", temp->Num_Train, temp->date_Begin, temp->time_Begin, temp->second_Begin);
			printf("����ǰ30mins���׼�Ʊ��^_^\n");
			Line();


			//���� Route.text �ļ�  ����
			temp = head_Route;
			fopen_s(&fptemp,"temp.txt","w"); // �������ļ�
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
			
			//�ر�
			fclose(fptemp);
			fclose(fpBookedPeo);
			free(head_Booked);//�ͷ�

			remove("Route.txt");
			rename("temp.txt","Route.txt");
				
		}
		else if(a == 0)
		{
			Line();
			temp = head_Route;
			goto start;
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
	//���� ���֤ �� ��ѡ ��ID
	//��Ʊ�󣬸���Roulist(��Ʊ�����ĸ���),����txt
	//���ѹ������� ɾ��ID�Ǹ��ڵ� ������txt
	//�����ж� �������Ƿ����Ҫ��(���ؽ���������) ����ÿһλ���� ͨ������ID���Ա�Level�������Ƿ�>0,������Զ����ж�Ʊ��
	// �����º�������ɾȥ�Ǹ��ڵ㣩�Լ�BookedPeo�������� ����򲹳ɹ�����Ϣ��д��򲹳ɹ�����txt �� �ɹ���Ʊ��txt�����¸��ǣ�
	// 
	//


}



//���˲�Ʊ
void Search_Inf()
{
	_Bool find = FALSE;
	long long int wait_ID;
start2:
	printf("������������֤���룺");
	scanf_s("%lld", &wait_ID);

	//�������֤��
	struct BookedPeo* temp_1;
	temp_1 = head_Booked;
	while (temp_1->next->next != NULL)
	{
		if (temp_1->Id == wait_ID)
		{
			find = TRUE;
			//�����Ϣ

		}
		temp_1 = temp_1->next;
	
	}
	if (temp_1->Id == wait_ID)//�ж����һ������
	{
		find = TRUE;
		//�����Ϣ

	}
	if (find == FALSE)
	{
		printf("���������֤�����Ƿ���ȷ����ѯ������ؼ�¼\n");
		printf("��������֤���������� 1");
		int a = 0;
		scanf_s("%d",&a);
		Line();
		if (a == 1)
		{
			goto start2;
		}
		else
		{
			return;
		}
	}
}


int main()
{
	//�ж��ļ��Ƿ����
	FindDatabase(&fpRoute, &fpBookedPeo, &fpWaitPeo);
	//·�߼��ؽ�����
	InitList_Route(&fpRoute,fpRoute);	
	//�����ѹ�Ʊ������
	InitList_BookedPeo();



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
			Search_Route();//1.��ѯ��Ʊ
			break;
		case 2:   
			//2.Ԥ����Ʊ
			Book_Ticket();
			break;
		case 3:   
			//3.�˶���Ʊ
			break;
		case 4:   
			//4.�򲹲�ѯ
			break;
		case 5:
			Search_Inf();
			break;
		case 9:
			system("CLS");
			AdminMenu(); //9.��̨����
			break;
		case 0:   //0.�˳�����
			printf("��ӭ�´�ʹ��\n");
			fclose(fpRoute);
			fclose(fpBookedPeo);
			fclose(fpWaitPeo);
			exit(0);
			break;

		default:
			printf("����������\n");
			Sleep(800);
			system("CLS");
			break;
		}
	}


	return 0;
}





