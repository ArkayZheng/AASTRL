#include "AASTRL.h"

typedef struct Sensor		//传感器簇
{
	int Identifier,		//指该簇对应环	
		Next_Step;		//指该簇传输信息所跨环数，用以更新信息素
	double	Information,		//指该簇信息量
		Distance,	//该簇到节点距离	
		Acreage,		//指该簇的面积
		Energy;		//指该簇拥有的所有能量
	double	*Pheromone,	//信息素
		*Heuristic_factor,	//启发因子
		*Probability;		//概率
	char Visited;
	struct Sensor *front,*rear;
}Sensor,*Sense;

typedef struct Queue		//队列
{
	Sense *top;
	Sense *base;
	int Queuesize;
}Queue;

//结点访问函数，用于检验
void Visit(Sense T)
{
	if (T == NULL)
	{
		printf("Error.\n");
	}
	else
	{
		printf("该簇位于第 %d 环\n",(T->Identifier));
		printf("该簇信息量为 %.4f\n",(T->Information));
		printf("该簇离基站距离为 %.4f\n",(T->Distance));
		printf("该簇面积为 %.4f\n",(T->Acreage));
		printf("该簇剩余能量为 %.4f\n\n",(T->Energy));
	}
}

//链表头结点置空
void Init_Sensor_Link(Sense *T)
{
	*T = NULL;
}

//生成链表
int Create_Sensor_Link(Sense *T_Start,Sense *T_End)		//T_Strat指链表头结点，T_End指链表尾结点
{
	int i,j;
	double k = 0;	//中间变量，概率计算公式中的分母
	Sense t,t_new;

	//初始化尾结点，尾结点指基站
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

	//初始化头结点
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

	for (i=R/omega; i>=1; i--)		//通过 R/omega 求环数，除了基站之外的环数
	{
		k = 0;

		t_new = (Sensor*)malloc(sizeof(Sensor));
		if(!(t_new))
			exit(-1);
		t_new->Identifier = R/omega - i + 1;		//该簇是第几环
		t_new->Information = 0;		//初始化信息量为零
		t_new->Distance = i * omega;	//该簇到基站距离
		t_new->Acreage = (3.14 * ((i*omega)*(i*omega) - ((i-1)*omega)*((i-1)*omega))) / 6;		//该簇的面积,形状为六分之一扇形
		t_new->Energy = t_new->Acreage * Sensor_number_per_square_meter * epsilon;		//求该簇总能量
		t_new->Visited = 0;

		//分配动态数组
		t_new->Pheromone = (double*)malloc((K+1) * sizeof(double));
		t_new->Heuristic_factor = (double*)malloc((K+1) * sizeof(double));
		t_new->Probability = (double*)malloc((K+1) * sizeof(double));
		
		//数组赋值
		for (j=0; j<=K; j++)
		{
			t_new->Pheromone[j] = 0;
			t_new->Heuristic_factor[j] = 0;
			t_new->Probability[j] = 0;
		}
		for (j=0; j<=K; j++)
		{
			//信息素初始值是该环自有信息量传至对应环所耗能的倒数，此处耗能指平均每个结点的耗能
			t_new->Pheromone[j] = 1 / (phi * (E_elec + j*omega * j*omega * epsilon_fs));
			//启发因子初始值是该环至对应环距离
			t_new->Heuristic_factor[j] = j * omega;

			if (t_new->Identifier+j > (R/omega+1))		//防止其最大跳数超过基站
			{
				t_new->Pheromone[j] = 0;
				t_new->Heuristic_factor[j] = 0;
			}
			
			k += pow(t_new->Pheromone[j],a) * pow(t_new->Heuristic_factor[j],b);
		}
		for (j=0; j<=K; j++)
		{
			//初始概率公式参见PPT
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

//销毁链表
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

//队列头结点置空
void Init_Queue(Queue *Q)
{
	Q = NULL;
}

//生成队列
int Create_Queue(Queue *Q)
{

	Q->base = (Sense*)malloc(500*sizeof(Sense));
	if(!(Q->base))
		exit(-1);
	Q->top = Q->base;
	Q->Queuesize = 500;
	return 1;
}

//销毁队列
void Destroy_Queue(Queue *Q)
{
	free(Q->base);
	Q->base = NULL;
	Q->top = NULL;
	Q->Queuesize = 0;
}

//进队列
int Push(Queue *Q,Sense T)
{
	*(Q->top) = T;
	Q->top++; 

	return 1;
}

//出队列
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

//生成随机数
double random()		//产生0-1的随机数，用于选择接下来要访问的环
{
	double number;
	number = rand() % 10000 + 1;
	number = number / 10000;

	return number;
}

//信息素更新，须写于传输之前
void Pheromone_Updating(Sense Source,int Start)
{
	Sense T = Source,t;
	int i,j,s = Start;		//在传输后需马上更新信息素，需要记录传输开始的环序号
	double k = 0,Transsmission_Cost = 0;

	//进入传输开端,
	for (i=1; i<s; i++)
	{
		T = T->rear;
	}

	//收集经过各环的ECR值
	t = T;
	while (t->Identifier + t->Next_Step != R/omega + 1)
	{
		//当前环的ECR值
		Transsmission_Cost += (t->Acreage * Sensor_number_per_square_meter * epsilon - t->Energy) / 
			(t->Acreage * Sensor_number_per_square_meter);

		//跳到下一环
		j = t->Next_Step;
		for (i=0; i<j; i++)
			t = t->rear;
	}

	//信息素更新
	t = T;
	while (t->Identifier + t->Next_Step != R/omega + 1)
	{
		//当前环信息素更新，概率更新
		t->Pheromone[t->Next_Step] += 1/Transsmission_Cost;
		for (i=0; i<=K; i++)
		{
			t->Pheromone[i] *= (1-gamma);
			k += pow(t->Pheromone[i],a) * pow(t->Heuristic_factor[i],b);
		}
		for (i=0; i<=K; i++)
			t->Probability[i] = pow(t->Pheromone[i],a) * pow(t->Heuristic_factor[i],b) / k;

		//跳到下一环
		j = t->Next_Step;
		for (i=0; i<j; i++)
			t = t->rear;
	}
}

//传输一次，注意每传输一次就要马上更新信息素
int Transmission(Sense Source,int *Start)
{
	Sense T = Source,
		  t;
	double Pro = 0,		//概率，随机数
		   k = 0;
	char All_Visited = 1;		//对于当前进行传输的结点，能访问的结点是否都已访问
	int i,j = 0;

	//选择一个尚未访问过的结点作为传输的开端，并记录
	while (T->Visited == 1)
	{
		T = T->rear;
		//若最内环也已访问，结束函数
		if (T->Identifier == R/omega)
			return 0;
	}
	*Start = T->Identifier;

	//若当前环仍有访问基站的可能性
	while (T->rear)
	{
		T->Visited = 1;
		k = 0;

		//确定该结点访问范围内所有结点是否都已访问
		t = T;
		for (i=1; i<=K; i++)
		{
			t = t->rear;
			if (t == NULL)
				i = K;
			else if (t->Visited == 0)
				All_Visited = 0;
		}

		//选择下一个访问结点
		//若能访问的结点皆已访问，则接下来访问的结点无限制
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
		//若能访问的结点未皆已访问，选择未访问的结点访问
		else
		{
			//对概率进行预处理
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
				//概率按比例放大
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
		//printf("从第 %d 环开始传输,应该传输到第 %d 环\n",T->Identifier,T->Next_Step + T->Identifier);

		//计算传到目标环所耗费的能量，所发送信息量为自身增加的信息量加上之前传输后积累的信息量
		T->Information += T->Acreage * Sensor_number_per_square_meter * phi;
		T->Energy  -= ((T->Next_Step*omega) * (T->Next_Step*omega) * epsilon_fs + E_elec) * T->Information;

		//传输到目标环，目标环信息量增加，能量减少
		t = T;
		for(i=0; i<T->Next_Step; i++)
			t = t->rear;
		t->Information += T->Information;
		t->Energy -= T->Information * E_elec;

		//正向蚂蚁移动到目标环，从目标环开始传输
		T = t;

		All_Visited = 1;
	}

	
	//总体传输仍未完成
	return 1;
}

//计算最大传输轮次以及剩余能量比率
void Performance_Evaluation(Sense source)
{
	Sense T = source;
	int Transsmission_Turns = 0,		//最大传输轮次
		j;
	double Rest_of_Energy,		//剩余能量比率
		Transsmission_Cost,		//传输一次耗能
		m = 0,n = 0;

	//最大传输轮次计算通过每环原有能量减去现有能量，得到传输一次消耗能量，再用原有能量除之
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
		m += T->Acreage * Sensor_number_per_square_meter * epsilon;		//m表示原有的总能量
		n += Transsmission_Cost * Transsmission_Turns;		//n表示在最大传输轮次后耗费的能量

		//信息量、能量、访问标签重置
		T->Information = 0;
		T->Energy = T->Acreage * Sensor_number_per_square_meter * epsilon;
		T->Visited = 0;
		
		T = T->rear;
	}
	Rest_of_Energy = 1 - n/m;

	printf("最大传输 %d 轮\n",Transsmission_Turns);
	printf("剩余能量比率为 %.4f \n",Rest_of_Energy);
}

//总体初始化
int Init(Sense *Source,Sense *Sink,Queue *Q)
{
	int boolean;

	srand((unsigned)time(NULL));		//随机数以时间做种

	Init_Sensor_Link(Source);
	boolean = Create_Sensor_Link(Source,Sink);
	if (boolean)
		printf("链表建立成功\n");
	else
		exit(0);

	Init_Queue(Q);
	boolean = Create_Queue(Q);
	if (boolean)
		printf("队列建立成功\n");
	else
		exit(0);

	return 1;
}

int main()
{
	int	i,
		boolean = 1,
		Start,
		Times = 200;		//迭代次数
	Sense Source,Sink;
	Queue Q;

	Init(&Source,&Sink,&Q);

	//printf("请输入迭代次数：	");
	//scanf("%d",&Times);

	for (i=1; i<=Times; i++)
	{	
		printf("\n第 %d 次传输开始\n",i);
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