#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<math.h>
#include<string.h>
#include <dirent.h>

struct binary{
	float x1,y1,z1;
	int r1,g1,b1,a,c;
	char str9;

}oku;
int main()
{
	FILE *fp1;

	DIR *dir;
	struct dirent *dirent;
	dir=opendir(".");
	int nktkontrol;


		while((dirent=readdir(dir))!=NULL)
			{
			char text[50],nkt[]={'.','n','k','t'};


			int len=strlen(dirent->d_name);


				if(len>4 && dirent->d_name[len -4]=='.'&& dirent->d_name[len -3]=='n'&& dirent->d_name[len -2]=='k'&& dirent->d_name[len -1]=='t' )
					{
						printf("Suan okunan dosya :%s\n",dirent->d_name);
						nktkontrol =1;
					}
				else{
					printf("\n>>>>NKT uzantisi degil : %s\n",dirent->d_name);
					nktkontrol = 0;
				}


		while(nktkontrol){




	strcpy(text,dirent->d_name);

	fp1=fopen(text,"r");
	char ch,str1,str2[20],str3[20],str4[20],str5[20],str6[20],str7[20],str8[20],str9[20],str[6];
	int a,c,dosyaninuzunlugu;

	int i=0,k1,k2,b1,b2;
	while(!feof(fp1))
	{
		ch=getc(fp1);
		if(ch=='\n')
		dosyaninuzunlugu++;

	}
	fseek(fp1,0L,SEEK_SET);
	double x[dosyaninuzunlugu];//dinamik dizi
	double y[dosyaninuzunlugu];
	double z[dosyaninuzunlugu];
	int rgb[3];

	int r,g,b;
	int sayac=0,sayac2=0;
	double enk,enb,toplam=0,ort=0;


	if(fp1==NULL){
		printf("Dosya okuma hatasi");
		continue;

	}

	fscanf(fp1,"%c %s %s %s\n",&str1,str2,str3,str4);
	printf("%c %s %s %s\n",str1,str2,str3,str4);
	fscanf(fp1,"%s %d\n",str5,&a);
	printf("%s %d\n",str5,a);
	fscanf(fp1,"%s %c %c %c %c %c %c\n",str6,&str[0],&str[1],&str[2],&str[3],&str[4],&str[5]);
	printf("%s %c %c %c %c %c %c\n",str6,str[0],str[1],str[2],str[3],str[4],str[5]);
	fscanf(fp1,"%s %d\n",str7,&c);
	printf("%s %d\n",str7,c);
	fscanf(fp1,"%s %s\n",str8,str9);
	printf("%s %s\n",str8,str9);

	if(a!=1)
	{printf("Dosya versiyonu uygun deðildir.");
	break;
	}


	while(!feof(fp1))
	{
		fscanf(fp1,"%lf %lf %lf %d %d %d\n",&x[i],&y[i],&z[i],&r,&g,&b);
       	//printf("%lf %lf %lf %d %d %d\n",x[i],y[i],z[i],r,g,b);
       	sayac++;
    	i++;

	}

	double uzunluk=0;double d1=0;int j=0;


	enk=90000;
	enb=0;
	int n;
	for( n=0;n<sayac;n++)
	{
		int m;

		for(m=1;m<sayac-1;m++)
		{	sayac2++;
			if(n!=m){
			d1=pow((x[n]-x[m]),2)+pow((y[n]-y[m]),2)+pow((z[n]-z[m]),2);
			uzunluk=sqrt(d1);
			toplam+=uzunluk;



				if(uzunluk<enk)
				{
					enk=uzunluk;
					k1=n;
					k2=m;

				}
				if(uzunluk>enb)
				{
					enb=uzunluk;
					b1=n;
					b2=m;

				}


			}

		}


	}
	printf("\n\n\nEn kucuk uzaklik :%lf  Elemanlar:%d. ve %d. noktalar.",enk,k1,k2);
	printf("\nEn buyuk uzaklik :%lf  Elemanlar:%d. ve %d. noktalar.",enb,b1,b2);
	

	ort=(toplam/sayac2);



	double zenk=90000,zenb=0;
	int k=0,zkordinatk,zkordinatb;
	while(z[k]!=0)
	{

			if(z[k]<zenk)
				{
					zenk=z[k];
					zkordinatk=k;


				}
			if(z[k]>zenb)
			{
				zenb=z[k];
				zkordinatb=k;

			}

		k++;
	}
	double zkenar=zenb-zenk;



	double yenk=90000,yenb=0;
	int l=0,ykordinatk=6,ykordinatb=6;
	while(y[l]!=0)
	{

			if(y[l]<yenk)
				{
					yenk=y[l];
					ykordinatk=l;


				}
			if(y[l]>yenb)
			{
				yenb=y[l];
				ykordinatb=l;

			}

		l++;
	}
	double ykenar=yenb-yenk;


	double xenk=90000,xenb=0;
	int p=0,xkordinatk,xkordinatb;
	while(x[p]!=0)
	{

			if(x[p]<xenk)
			{
				xenk=x[p];
				xkordinatk=p;


			}
			if(x[p]>xenb)
			{
				xenb=x[p];
				xkordinatb=p;


			}

		p++;
	}
	double xkenar=xenb-xenk;

	if(zkenar>xkenar && zkenar>ykenar)
	{
		printf("\n\nKupun noktalari:\n");

		printf("\n1.nokta=%lf %lf %lf    ",xenk,yenk,zenk);
		printf("\n2.nokta=%lf %lf %lf    ",xenk+zkenar,yenk,zenk);
		printf("\n3.nokta=%lf %lf %lf    ",xenk,yenk+zkenar,zenk);
		printf("\n4.nokta=%lf %lf %lf    ",xenk,yenk,zenk+zkenar);
		printf("\n5.nokta=%lf %lf %lf    ",xenk+zkenar,yenk+zkenar,zenk);
		printf("\n6.nokta=%lf %lf %lf    ",xenk,yenk+zkenar,zenk+zkenar);
		printf("\n7.nokta=%lf %lf %lf    ",xenk+zkenar,yenk,zenk+zkenar);
		printf("\n8.nokta=%lf %lf %lf    ",xenk+zkenar,yenk+zkenar,zenk+zkenar);

	}


	else if(ykenar>xkenar && ykenar>zkenar)
	{
		printf("\n\nKupun noktalari:\n");

		printf("\n1.nokta=%lf %lf %lf    ",xenk,yenk,zenk);
		printf("\n2.nokta=%lf %lf %lf    ",xenk+ykenar,yenk,zenk);
		printf("\n3.nokta=%lf %lf %lf    ",xenk,yenk+ykenar,zenk);
		printf("\n4.nokta=%lf %lf %lf    ",xenk,yenk,zenk+ykenar);
		printf("\n5.nokta=%lf %lf %lf    ",xenk+ykenar,yenk+ykenar,zenk);
		printf("\n6.nokta=%lf %lf %lf    ",xenk,yenk+ykenar,zenk+ykenar);
		printf("\n7.nokta=%lf %lf %lf    ",xenk+ykenar,yenk,zenk+ykenar);
		printf("\n8.nokta=%lf %lf %lf    ",xenk+ykenar,yenk+ykenar,zenk+ykenar);

	}

	else
	{
		printf("\n\nKupun noktalari:\n");

		printf("\n1.nokta=%lf %lf %lf    ",xenk,yenk,zenk);
		printf("\n2.nokta=%lf %lf %lf    ",xenk+xkenar,yenk,zenk);
		printf("\n3.nokta=%lf %lf %lf    ",xenk,yenk+xkenar,zenk);
		printf("\n4.nokta=%lf %lf %lf    ",xenk,yenk,zenk+xkenar);
		printf("\n5.nokta=%lf %lf %lf    ",xenk+xkenar,yenk+xkenar,zenk);
		printf("\n6.nokta=%lf %lf %lf    ",xenk,yenk+xkenar,zenk+xkenar);
		printf("\n7.nokta=%lf %lf %lf    ",xenk+xkenar,yenk,zenk+xkenar);
		printf("\n8.nokta=%lf %lf %lf    ",xenk+xkenar,yenk+xkenar,zenk+xkenar);


	}
	/*double sonuc=(toplam/(((sayac)*(sayac-1))/2));
    printf("%s nin ortalamasi: %lf\n",sonuc);*/

//KURE
    fseek(fp1,0L,SEEK_SET);
	double xkul,ykul,zkul,yaricap;
	printf("\n\nKurenin yaricapini giriniz :");
	scanf("%lf",&yaricap);

	printf("\nKurenin 3 boyutlu koordinatini giriniz:\n");
	printf("x noktasini giriniz :");
	scanf("%lf",&xkul);

	printf("y noktasini giriniz :");
	scanf("%lf",&ykul);

	printf("z noktasini giriniz :");
	scanf("%lf",&zkul);

	fscanf(fp1,"%c %s %s %s\n",&str1,str2,str3,str4);
	printf("%c %s %s %s\n",str1,str2,str3,str4);
	fscanf(fp1,"%s %d\n",str5,&a);
	printf("%s %d\n",str5,a);
	fscanf(fp1,"%s %c %c %c %c %c %c\n",str6,&str[0],&str[1],&str[2],&str[3],&str[4],&str[5]);
	printf("%s %c %c %c %c %c %c\n",str6,str[0],str[1],str[2],str[3],str[4],str[5]);
	fscanf(fp1,"%s %d\n",str7,&c);
	printf("%s %d\n",str7,c);
	fscanf(fp1,"%s %s\n",str8,str9);
	printf("%s %s\n",str8,str9);
	printf("\nKurenin icinde kalan nokta ve numaralari:\n");
	int u=0,sayacu=0;
	double d2;

	while(!feof(fp1))
	{

		fscanf(fp1,"%lf %lf %lf %d %d %d",&x[u],&y[u],&z[u],&r,&g,&b);

       	//pow(d2,2)=(pow((x[u]-xkul),2)+pow((y[u]-ykul),2)+pow((z[u]-zkul),2));
    	if(yaricap>=sqrt(pow((x[u]-xkul),2)+pow((y[u]-ykul),2)+pow((z[u]-zkul),2)))
    		{
    			printf("%d.[%lf %lf %lf]\n",(u+1),x[u],y[u],z[u]);
				sayacu++;
			}


		u++;

	}
	printf("\n->%d nokta bu kurenin icindedir.\n",sayacu);
	printf("Ortalama deger=%lf\n\n",ort);

	fseek(fp1,0L,SEEK_SET);

		break;	}}

	return 0;

}
