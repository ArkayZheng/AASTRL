#include "AASTRL.h"

typedef struct Sensor		//��������
{
	int Identifier,		//ָ�ôض�Ӧ��	
		Next_Step;		//ָ�ôش�����Ϣ���绷�������Ը�����Ϣ��
	double	Information,		//ָ�ô���Ϣ��
		Distance,	//�ôص��ڵ����	
		Acreage,		//ָ�ôص����
		Energy;		//ָ�ô�ӵ�е���������
	double	*Pheromone,	//��Ϣ��
		*Heuristic_factor,	//��������
		*Probability;		//����
	char Visited;
	struct Sensor *front,*rear;
}Sensor,*Sense;

typedef struct Queue		//����
{
	Sense *top;
	Sense *base;
	int Queuesize;
}Queue;

//�����ʺ��������ڼ���
void Visit(Sense T)
{
	if (T == NULL)
	{
		printf("Error.\n");
	}
	else
	{
		printf("�ô�λ�ڵ� %d ��\n",(T->Identifier));
		printf("�ô���Ϣ��Ϊ %.4f\n",(T->Information));
		printf("�ô����վ����Ϊ %.4f\n",(T->Distance));
		printf("�ô����Ϊ %.4f\n",(T->Acreage));
		printf("�ô�ʣ������Ϊ %.4f\n\n",(T->Energy));
	}
}

//����ͷ����ÿ�
void Init_Sensor_Link(Sense *T)
{
	*T = NULL;
}

//��������
int Create_Sensor_Link(Sense *T_Start,Sense *T_End)		//T_Stratָ����ͷ��㣬T_Endָ����β���
{
	int i,j;
	double k = 0;	//�м���������ʼ��㹫ʽ�еķ�ĸ
	Sense t,t_new;

	//��ʼ��β��㣬β���ָ��վ
	*T_End = (Sensor*)malloc(sizeof(Sensor));
	if(!(*T_End))
		exit(-1);
	(*T_End)->Identifier = R/omega + 1;
	(*T_End)->Information = 0;
	(*T_End)->Distance = 0;
	(*T_End)->Acreage = 0;
	(*T_End)->Energy = 0;
	(*T_End)->Visited = 0;
	(*T_End)->Pheromone = NULL;
	(*T_End)->Heuristic_factor = NULL;
	(*T_End)->Probability = NULL;

	//��ʼ��ͷ���
	*T_Start = (Sensor*)malloc(sizeof(Sensor));
	if(!(*T_Start))
		exit(-1);
	(*T_Start)->Identifier = 0;
	(*T_Start)->Information = 0;
	(*T_Start)->Distance = 0;
	(*T_Start)->Acreage = 0;
	(*T_Start)->Energy = 0;
	(*T_Start)->Visited = 0;
	(*T_Start)->Pheromone = NULL;
	(*T_Start)->Heuristic_factor = NULL;
	(*T_Start)->Probability = NULL;

	t = *T_Start;

	for (i=R/omega; i>=1; i--)		//ͨ�� R/omega ���������˻�վ֮��Ļ���
	{
		k = 0;

		t_new = (Sensor*)malloc(sizeof(Sensor));
		if(!(t_new))
			exit(-1);
		t_new->Identifier = R/omega - i + 1;		//�ô��ǵڼ���
		t_new->Information = 0;		//��ʼ����Ϣ��Ϊ��
		t_new->Distance = i * omega;	//�ôص���վ����
		t_new->Acreage = (3.14 * ((i*omega)*(i*omega) - ((i-1)*omega)*((i-1)*omega))) / 6;		//�ôص����,��״Ϊ����֮һ����
		t_new->Energy = t_new->Acreage * Sensor_number_per_square_meter * epsilon;		//��ô�������
		t_new->Visited = 0;

		//���䶯̬����
		t_new->Pheromone = (double*)malloc((K+1) * sizeof(double));
		t_new->Heuristic_factor = (double*)malloc((K+1) * sizeof(double));
		t_new->Probability = (double*)malloc((K+1) * sizeof(double));
		
		//���鸳ֵ
		for (j=0; j<=K; j++)
		{
			t_new->Pheromone[j] = 0;
			t_new->Heuristic_factor[j] = 0;
			t_new->Probability[j] = 0;
		}
		for (j=0; j<=K; j++)
		{
			//��Ϣ�س�ʼֵ�Ǹû�������Ϣ��������Ӧ�������ܵĵ������˴�����ָƽ��ÿ�����ĺ���
			t_new->Pheromone[j] = 1 / (phi * (E_elec + j*omega * j*omega * epsilon_fs));
			//�������ӳ�ʼֵ�Ǹû�����Ӧ������
			t_new->Heuristic_factor[j] = j * omega;

			if (t_new->Identifier+j > (R/omega+1))		//��ֹ���������������վ
			{
				t_new->Pheromone[j] = 0;
				t_new->Heuristic_factor[j] = 0;
			}
			
			k += pow(t_new->Pheromone[j],a) * pow(t_new->Heuristic_factor[j],b);
		}
		for (j=0; j<=K; j++)
		{
			//��ʼ���ʹ�ʽ�μ�PPT
			t_new->Probability[j] = pow(t_new->Pheromone[j],a) * pow(t_new->Heuristic_factor[j],b) / k;
		}

		t_new->rear = NULL;
		t->rear = t_new;
		t_new->front = t;

		/*for (j=0; j<=K; j++)
		{
		printf("%.8f\n",t_new->Probability[j]);
		}
		Visit(t_new);*/

		t = t->rear;
	}

	t->rear = (*T_End);
	(*T_End)->front = t;
	(*T_End)->rear = NULL;
	(*T_Start)->front = NULL;

	return 1;
}

//��������
void Destroy_Sensor_Link(Sense *T)
{
	if(*T)
	{
		if((*T)->rear)
			Destroy_Sensor_Link(&((*T)->rear));
		free(*T);
		*T = NULL;
	}
}

//����ͷ����ÿ�
void Init_Queue(Queue *Q)
{
	Q = NULL;
}

//���ɶ���
int Create_Queue(Queue *Q)
{

	Q->base = (Sense*)malloc(500*sizeof(Sense));
	if(!(Q->base))
		exit(-1);
	Q->top = Q->base;
	Q->Queuesize = 500;
	return 1;
}

//���ٶ���
void Destroy_Queue(Queue *Q)
{
	free(Q->base);
	Q->base = NULL;
	Q->top = NULL;
	Q->Queuesize = 0;
}

//������
int Push(Queue *Q,Sense T)
{
	*(Q->top) = T;
	Q->top++; 

	return 1;
}

//������
int Pop(Queue *Q,Sense *T)
{
	if(Q->top == Q->base)
	{
		printf("Queue is empty.\n");
		return 0;
	}
	else
	{
		
		(*T) = *(Q->base);
		Q->base++;
		return 1;
	}
}

//���������
double random()		//����0-1�������������ѡ�������Ҫ���ʵĻ�
{
	double number;
	number = rand() % 10000 + 1;
	number = number / 10000;

	return number;
}

//��Ϣ�ظ��£���д�ڴ���֮ǰ
void Pheromone_Updating(Sense Source,int Start)
{
	Sense T = Source,t;
	int i,j,s = Start;		//�ڴ���������ϸ�����Ϣ�أ���Ҫ��¼���俪ʼ�Ļ����
	double k = 0,Transsmission_Cost = 0;

	//���봫�俪��,
	for (i=1; i<s; i++)
	{
		T = T->rear;
	}

	//�ռ�����������ECRֵ
	t = T;
	while (t->Identifier + t->Next_Step != R/omega + 1)
	{
		//��ǰ����ECRֵ
		Transsmission_Cost += (t->Acreage * Sensor_number_per_square_meter * epsilon - t->Energy) / 
			(t->Acreage * Sensor_number_per_square_meter);

		//������һ��
		j = t->Next_Step;
		for (i=0; i<j; i++)
			t = t->rear;
	}

	//��Ϣ�ظ���
	t = T;
	while (t->Identifier + t->Next_Step != R/omega + 1)
	{
		//��ǰ����Ϣ�ظ��£����ʸ���
		t->Pheromone[t->Next_Step] += 1/Transsmission_Cost;
		for (i=0; i<=K; i++)
		{
			t->Pheromone[i] *= (1-gamma);
			k += pow(t->Pheromone[i],a) * pow(t->Heuristic_factor[i],b);
		}
		for (i=0; i<=K; i++)
			t->Probability[i] = pow(t->Pheromone[i],a) * pow(t->Heuristic_factor[i],b) / k;

		//������һ��
		j = t->Next_Step;
		for (i=0; i<j; i++)
			t = t->rear;
	}
}

//����һ�Σ�ע��ÿ����һ�ξ�Ҫ���ϸ�����Ϣ��
int Transmission(Sense Source,int *Start)
{
	Sense T = Source,
		  t;
	double Pro = 0,		//���ʣ������
		   k = 0;
	char All_Visited = 1;		//���ڵ�ǰ���д���Ľ�㣬�ܷ��ʵĽ���Ƿ��ѷ���
	int i,j = 0;

	//ѡ��һ����δ���ʹ��Ľ����Ϊ����Ŀ��ˣ�����¼
	while (T->Visited == 1)
	{
		T = T->rear;
		//�����ڻ�Ҳ�ѷ��ʣ���������
		if (T->Identifier == R/omega)
			return 0;
	}
	*Start = T->Identifier;

	//����ǰ�����з��ʻ�վ�Ŀ�����
	while (T->rear)
	{
		T->Visited = 1;
		k = 0;

		//ȷ���ý����ʷ�Χ�����н���Ƿ��ѷ���
		t = T;
		for (i=1; i<=K; i++)
		{
			t = t->rear;
			if (t == NULL)
				i = K;
			else if (t->Visited == 0)
				All_Visited = 0;
		}

		//ѡ����һ�����ʽ��
		//���ܷ��ʵĽ����ѷ��ʣ�����������ʵĽ��������
		if (All_Visited == 1)
		{
			Pro = random();
			for (i=1; i<=K; i++)
			{
				if (Pro<= T->Probability[i])
				{
					j = i;
					i = K;
				}
				else
					Pro -= T->Probability[i];
			}
		}
		//���ܷ��ʵĽ��δ���ѷ��ʣ�ѡ��δ���ʵĽ�����
		else
		{
			//�Ը��ʽ���Ԥ����
			t = T;
			for (i=1; i<=K; i++)
			{
				t = t->rear;
				if (t == NULL)
					i = K;
				else if (t->Visited == 0)
					k += T->Probability[i];
			}

			t = T;
			Pro = random();
			for (i=1; i<=K; i++)
			{
				t = t->rear;
				//���ʰ������Ŵ�
				if (Pro <= (T->Probability[i]/k) && (t->Visited == 0))
				{
					j = i;
					i = K;
				}
				else if (t->Visited == 0)
					Pro -= (T->Probability[i]/k);
			}
		}
		T->Next_Step = j;
		//printf("�ӵ� %d ����ʼ����,Ӧ�ô��䵽�� %d ��\n",T->Identifier,T->Next_Step + T->Identifier);

		//���㴫��Ŀ�껷���ķѵ���������������Ϣ��Ϊ�������ӵ���Ϣ������֮ǰ�������۵���Ϣ��
		T->Information += T->Acreage * Sensor_number_per_square_meter * phi;
		T->Energy  -= ((T->Next_Step*omega) * (T->Next_Step*omega) * epsilon_fs + E_elec) * T->Information;

		//���䵽Ŀ�껷��Ŀ�껷��Ϣ�����ӣ���������
		t = T;
		for(i=0; i<T->Next_Step; i++)
			t = t->rear;
		t->Information += T->Information;
		t->Energy -= T->Information * E_elec;

		//���������ƶ���Ŀ�껷����Ŀ�껷��ʼ����
		T = t;

		All_Visited = 1;
	}

	
	//���崫����δ���
	return 1;
}

//����������ִ��Լ�ʣ����������
void Performance_Evaluation(Sense source)
{
	Sense T = source;
	int Transsmission_Turns = 0,		//������ִ�
		j;
	double Rest_of_Energy,		//ʣ����������
		Transsmission_Cost,		//����һ�κ���
		m = 0,n = 0;

	//������ִμ���ͨ��ÿ��ԭ��������ȥ�����������õ�����һ����������������ԭ��������֮
	Transsmission_Cost = T->Acreage * Sensor_number_per_square_meter * epsilon - T->Energy;
	Transsmission_Turns = (int)((T->Acreage * Sensor_number_per_square_meter * epsilon) / Transsmission_Cost);
	T = T->rear;

	while (T->rear)
	{
		Transsmission_Cost = T->Acreage * Sensor_number_per_square_meter * epsilon - T->Energy;
		j = (int)((T->Acreage * Sensor_number_per_square_meter * epsilon) / Transsmission_Cost);
		if (j < Transsmission_Turns)
			Transsmission_Turns = j;

		T = T->rear;
	}

	T = source;
	while (T->rear)
	{
		Transsmission_Cost = T->Acreage * Sensor_number_per_square_meter * epsilon - T->Energy;
		m += T->Acreage * Sensor_number_per_square_meter * epsilon;		//m��ʾԭ�е�������
		n += Transsmission_Cost * Transsmission_Turns;		//n��ʾ��������ִκ�ķѵ�����

		//��Ϣ�������������ʱ�ǩ����
		T->Information = 0;
		T->Energy = T->Acreage * Sensor_number_per_square_meter * epsilon;
		T->Visited = 0;
		
		T = T->rear;
	}
	Rest_of_Energy = 1 - n/m;

	printf("����� %d ��\n",Transsmission_Turns);
	printf("ʣ����������Ϊ %.4f \n",Rest_of_Energy);
}

//�����ʼ��
int Init(Sense *Source,Sense *Sink,Queue *Q)
{
	int boolean;

	srand((unsigned)time(NULL));		//�������ʱ������

	Init_Sensor_Link(Source);
	boolean = Create_Sensor_Link(Source,Sink);
	if (boolean)
		printf("�������ɹ�\n");
	else
		exit(0);

	Init_Queue(Q);
	boolean = Create_Queue(Q);
	if (boolean)
		printf("���н����ɹ�\n");
	else
		exit(0);

	return 1;
}

int main()
{
	int	i,
		boolean = 1,
		Start,
		Times = 200;		//��������
	Sense Source,Sink;
	Queue Q;

	Init(&Source,&Sink,&Q);

	//printf("���������������	");
	//scanf("%d",&Times);

	for (i=1; i<=Times; i++)
	{	
		printf("\n�� %d �δ��俪ʼ\n",i);
		boolean = 1;
		while (boolean)
		{
			boolean = Transmission(Source->rear,&Start);
			Pheromone_Updating(Source->rear,Start);
		}
		Performance_Evaluation(Source->rear);
	}

	Destroy_Sensor_Link(&Source);
	Destroy_Queue(&Q);

	return 0;
}