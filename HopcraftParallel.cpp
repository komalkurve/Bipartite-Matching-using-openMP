#include<bits/stdc++.h>
#include<omp.h>
using namespace std;

vector<pair<int, int> > match,edge;
vector<pair<int, int> > pv;
std::map<int,vector<int> > L;
int k;
int leftg[100],m,rightg[100],n,graph[100][100],deg[200];
int visited[200];
void degree()
{
  #pragma omp ++ for
  for(int i=0;i<m+n;i++)
  {
  	for(int j=0;j<i;j++)
  	{
  	  if(graph[i][j]==1)
  	     {
  	     	deg[i]++;
  	     	deg[j]++;
  	    }
  	     
	}
  }
}
void update(int u,int visited[],int x)
{
	int id;
//	cout<<"\nUpdate vertex : "<<u<<endl;
	id = visited[u];
	//#pragma omp critical
	  visited[u] +=1;
//	cout<<"\nVisited 1: "<<u<<" "<<visited[u]<<endl;
	if(id==0)
	{
		for(int v=0;v<m+n ;v++)
		 if(graph[u][v]==1 && visited[v]==0)
		   {
		   	int temp = visited[v];
		   #pragma omp critical
		   	  visited[v] +=1;
	        if(temp==0)
	        {
	          match.push_back(make_pair(u,v));
	          cout<<"\nMatched vertex are u and v:" <<u<<" "<<v<<endl;
	          for(int w=0;w<m+n;w++)
			   if(graph[v][w]==1)
			    {
//			    	cout<<"\nValue of w :" <<w<<" and degree : " <<deg[w]<<"  visited: "<<visited[w]<<endl;
			    	int temp1 = deg[w];
	   	          #pragma omp critical
	                deg[w] +=-1;
	                if(temp1==2)
	                {
//	                 cout<<"\nValue of w :" <<w<<" and degree : " <<deg[w]<<"  visited: "<<visited[w]<<endl;

					  update(w,visited,x);
			        }
				 } 
			}
			break;
		   }
	}
}
void Init()
{
	//int visited[m+n];

	list<int> q1;
	list<int> q2;
    #pragma omp parallel for
     for(int i=0;i<m+n;i++)
        visited[i]=0;
     #pragma omp parallel for
      for(int i=0;i<m;i++)
      {
      	int u = leftg[i];
      	if (deg[u]==1)
      	{
      	  	q1.push_back(u);
		}
		else
		   q2.push_back(u);
	   }
	   //int it;
	   //#pragma omp parallel for
	   for(list<int>::iterator it =q1.begin();it!=q1.end(); ++it)
	      update(*it,visited,m+n);
         //#pragma omp parallel for
	   for(list<int>::iterator it =q2.begin();it!=q2.end(); ++it)
	      {
//		  cout<<"\nVertex to be added : "<<*it<<"   "<<visited[*it]<<endl;
		  update(*it,visited,m+n);
		
	      }
}
int check(int v)
{
	for(int i=0;i<match.size();i++)
	  if(match[i].first==v || match[i].second==v)
	   return 1;
	return 0;
}


void bfs()
{
    cout<<"calling matching"<<endl;
	for(int i=0;i<m;i++)
	{
		if(!check(leftg[i]))
		{
			cout<<"gsg";
	 	   L[0].push_back(leftg[i]);
			cout<<"fsf";		
		}
	}
    k=0;
	cout<<"aftere matching"<<endl;
	while(true)
	{
	
	        L[k+1].clear();
	        L[k+2].clear();
           
  
            for (vector<int>::const_iterator i=L[k].begin();i!=L[k].end();++i)
            {
            	
                int u = *i;
                for(int v=0;v<m+n;v++)
                {
                	if(graph[u][v]==1)
                	{
                		int temp = visited[v];
                		#pragma omp critical
                		  visited[v] +=1;
                		  if(temp==0)
                		  {
                		  	L[k+1].push_back(v);
                		  	edge.push_back(make_pair(u,v));
                		  	if (check(v))
                		  	{
                		  		int mv;
                		  	    for(int i=0;i<match.size();i++)
	  								   {
	  								    if(match[i].first==v)
									        mv = match[i].second;
									    else if(match[i].second==v)
	  								         mv = match[i].first;
                		  		       }
								L[k+2].push_back(mv);
                		  		edge.push_back(make_pair(v,mv));
							  }
						  }
					}
				}
 
               
            }
            int flag=0; 
                  
            for (vector<int>::const_iterator i=L[k+1].begin();i!=L[k+1].end();++i)
                {
                	int u = *i;
				     if(!check(u))
				      flag=1;
			    }
	    	if(L[k+1].empty() || flag==1 )
	    	  return;
		    else
		      k = k+2;
		               
	}
	
	
}


int dfs(int node)
{
	
	int u=node;
	for (int v=0;v<m+n;v++)
	 {
	 	
	 	if(graph[u][v]==1){
	 		
	 		int temp = visited[v];
	 		#pragma omp critical
	 		visited[v]+=1;
	 		
			 if(temp==0)
	 		{
	 		
	 			if(check(v))
	 			  	pv.push_back(make_pair(u,v));
	 			
			 }
			else
			{
				
				int index = dfs(v);
				if(index != -2)
				 	pv.push_back(make_pair(v,index));;
			}
		 }
	 }
	 return -2;
}

void hopcraft()
{
	cout<<"calling init"<<endl;
	Init();
	cout<<"after"<<endl;
	int next[m+n];
	vector<pair<int, int> > p;
	do
	{
		cout<<"inside whilw"<<endl;
		for(int i=0;i<m;i++)
	  	{
	  		int u=leftg[i];
	  	    next[u]=1;//TODO whats the use of this
	 	 }
	 	for(int w=0;w<m+n;w++)
	 	   visited[w]=0;
	 	 
	 	
	 	cout<<"calling bfs0"<<endl;
	    bfs();
	    cout<<"agfter"<<endl;
	 	
	 	for(int w=0;w<m+n;w++)
	 	   visited[w]=0;
	    
	   
	    vector<int> path = L[k];
	   // cout<<"\nSize : "<<path.size()<<endl; 
        for (vector<int>::const_iterator i=path.begin();i!=path.end();++i)
		{
          int v = *i;
          cout<<"calling dsfs : "<<v<<endl;
		  dfs(v);
		  cout<<"After dfs "<<endl;
		  p.insert(p.end(), pv.begin(), pv.end());	
		  pv.clear();  
		  
		}
		
	 	 match.insert(match.end(), p.begin(), p.end());	 
	 	
    }while(!p.empty());
}


int main()
{
	
	m=4;
    n=4;
	for(int i=0;i<m+n;i++)
	  for(int j=0;j<m+n;j++)
	  graph[i][j]=0;
	  
   graph[0][3]=1;
   graph[0][5]=1;
   graph[2][1]=1;
   graph[4][3]=1;
   graph[6][3]=1;
   graph[6][7]=1;

   graph[3][0]=1;
   graph[5][0]=1;
   graph[1][2]=1;
   graph[3][4]=1;
   graph[3][6]=1;
   graph[7][6]=1;

   leftg[0]=0;
   leftg[1]=2;
   leftg[2]=4;
   leftg[3]=6;
/*	m=2;
	n=2;
	for(int i=0;i<m+n;i++)
	  for(int j=0;j<m+n;j++)
	  graph[i][j]=0;
     
    graph[0][1]=1;
   
    graph[2][3]=1;
    
    
    graph[1][0]=1;
  
    graph[3][2]=1;
  
    leftg[0]=0;
    leftg[1]=2;
    rightg[0]=1;
    rightg[1]=3;
   */
    /*  graph
    0----------1
    		   -
    		-
    	-
     2---------3
     */
     degree();
    cout<<"calling hopcraft"<<endl;
	hopcraft();
     cout<<"\nMatching is :"<<endl;
     for(int i=0;i<match.size();i++)
	   cout<<"\n"<<match[i].first<<" ---> "<<match[i].second<<endl;
	  

     return 0;

}
