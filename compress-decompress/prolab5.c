#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>

#define OFFSETBITS 5
#define LENGTHBITS (8-OFFSETBITS)

#define OFFSETMASK ((1 << (OFFSETBITS)) - 1)
#define LENGTHMASK ((1 << (LENGTHBITS)) - 1)

#define GETOFFSET(x) (x >> LENGTHBITS)
#define GETLENGTH(x) (x & LENGTHMASK)
#define OFFSETLENGTH(x,y) (x << LENGTHBITS | y)

struct token {
    uint8_t offset_len;
    char c;
};


/*
* iki string'in ilk kaç karakteri özdeş?
* maksimum limit sayısı kadar kontrol yapar.
*/
inline int prefix_match_length(char *s1, char *s2, int limit)
{
    int len = 0;

    while (*s1++ == *s2++ && len < limit)
        len++;

    return len;
}

/*
* memcpy fonksiyonu ile benzer. Byte düzeyinde
* kopyalama yapma garantisi olduğu için, bu
* versiyonu kullanıyoruz.
*/
inline void lz77memcpy(char *s1, char *s2, int size)
{
    while (size--)
        *s1++ = *s2++;
}

/*
* token array'ini, karakter array'ine dönüştürür.
*/
char *decode(struct token *tokens, int numTokens, int *pcbDecoded)
{
    // hafızada ayırdığımız kapasite
    int cap = 1 << 3;

    // kullanılan byte sayısı
    *pcbDecoded = 0;

    // hafızada yer ayır
    char *decoded = malloc(cap);

    int i;
    for (i = 0; i < numTokens; i++)
    {
        // token'in içinden offset, length ve char
        // değerlerini oku
        int offset = GETOFFSET(tokens[i].offset_len);
        int len = GETLENGTH(tokens[i].offset_len);
        char c = tokens[i].c;

        // gerekirse kapasite artır.
        if (*pcbDecoded + len + 1 > cap)
        {
            cap = cap << 1;
            decoded = realloc(decoded, cap);
        }

        // eğer length 0 değilse, gösterilen karakteleri
        // kopyala
        if (len > 0)
        {
            lz77memcpy(&decoded[*pcbDecoded], &decoded[*pcbDecoded - offset], len);
        }

        // kopyalanan karakter kadar ileri sar
        *pcbDecoded += len;

        // tokenin içindeki karateri ekle.
        decoded[*pcbDecoded] = c;

        // 1 adım daha ileri sar.
        *pcbDecoded = *pcbDecoded + 1;
    }

    return decoded;
}
/*
* LZ77 ile sıkıştırılacak bir metin alır.
* token array'i döndürür.
* Eğer numTokens NULL değilse, token sayısını
* numTokens ile gösterilen yere kaydeder.
*/
struct token *encode(char *text, int limit, int *numTokens)
{
    // cap (kapasite) hafızada kaç tokenlik yer ayırdığımız.
    int cap = 1 << 3;

    // kaç token oluşturduğumuz.
    int _numTokens = 0;

    // oluşturulacak token
    struct token t;

    // lookahead ve search buffer'larının başlangıç pozisyonları
    char *lookahead, *search;

    // tokenler için yer ayır.
    struct token *encoded = malloc(cap * sizeof(struct token));

    // token oluşturma döngüsü
    for (lookahead = text; lookahead < text + limit; lookahead++)
    {
        // search buffer'ı lookahead buffer'ın 31 (OFFSETMASK) karakter
        // gerisine koyuyoruz.
        search = lookahead - OFFSETMASK;

        // search buffer'ın metnin dışına çıkmasına engel ol.
        if (search < text)
            search = text;

        // search bufferda bulunan en uzun eşleşmenin
        // boyutu
        int max_len = 0;

        // search bufferda bulunan en uzun eşleşmenin
        // pozisyonu
        char *max_match = lookahead;

        // search buffer içinde arama yap.
        for (; search < lookahead; search++)
        {
            int len = prefix_match_length(search, lookahead, LENGTHMASK);

            if (len > max_len)
            {
                max_len = len;
                max_match = search;
            }
        }

        /*
        * ! ÖNEMLİ !
        * Eğer eşleşmenin içine metnin son karakteri de dahil olmuşsa,
        * tokenin içine bir karakter koyabilmek için, eşleşmeyi kısaltmamız
        * gerekiyor.
        */
        if (lookahead + max_len >= text + limit)
        {
            max_len = text + limit - lookahead - 1;
        }


        // bulunan eşleşmeye göre token oluştur.
        t.offset_len = OFFSETLENGTH(lookahead - max_match, max_len);
        lookahead += max_len;
        t.c = *lookahead;

        // gerekirse, hafızada yer aç
        if (_numTokens + 1 > cap)
        {
            cap = cap << 1;
            encoded = realloc(encoded, cap * sizeof(struct token));
        }

        // oluşturulan tokeni, array'e kaydet.
        encoded[_numTokens++] = t;
    }

    if (numTokens)
        *numTokens = _numTokens;

    return encoded;
}

// bir dosyanın tamamını tek seferde
// okur. Büyük dosyaları okumak için uygun
// değildir.
char *file_read(FILE *f, int *size)
{
    char *content;
    fseek(f, 0, SEEK_END);
    *size = ftell(f);
    content = malloc(*size);
    fseek(f, 0, SEEK_SET);
    fread(content, 1, *size, f);
    return content;
}

int main(void)
{
    FILE *f;
    int metin_boyutu = 11, token_sayisi, decode_boyutu;
    char *kaynak_metin = "abrakadabra", *decoded_metin = "";
    struct token *encoded_metin;

    if (f = fopen("metin.txt", "rb"))
    {
        kaynak_metin = file_read(f, &metin_boyutu);
        fclose(f);
    }



    encoded_metin = encode(kaynak_metin, metin_boyutu, &token_sayisi);

    if (f = fopen("encoded.txt", "wb"))
    {
        fwrite(encoded_metin, sizeof(struct token), token_sayisi, f);
        fclose(f);
    }

    decoded_metin = decode(encoded_metin, token_sayisi, &decode_boyutu);

    if (f = fopen("decoded.txt", "wb"))
    {
        fwrite(decoded_metin, 1, decode_boyutu, f);
        fclose(f);
    }

    printf("Orjinal Boyut: %d, Encode Boyutu: %d, Decode Boyutu: %d\n", metin_boyutu, token_sayisi * sizeof(struct token), decode_boyutu);

   char str[13]="abracadabra";
	char sb[10]="",lb[10]="";
	int i,j,c=0,k=0,m,l,len,flag;
	//system("cls");
	int eleman=(sizeof sb/sizeof*sb);
	int eleman2=(sizeof lb/sizeof*lb);
	printf("%d\n",eleman);
	strcpy(str,"abracadabra");
	//cout<<"Given string is: "<<str;

	printf("\nGiven String is:%s",str);


	for(i=0;i<5;i++)
	{
		lb[i]=str[i];
	}
	for(i=0;i<11;i++)
	{
		str[i]=str[i+5];
	}

	for(m=0;m<6;m++)
	{
		i=0;
		j=0;
		flag=0;
		len=strlen(sb);
		//cout<<"\nlength of sb : "<<len;
		//printf("\nlength of sb:%d",sb);
		while(flag==0 && i<len)
		{
			if(sb[i]==lb[0])
			{
				flag=1;
				for(l=0;l<len;l++)
				{
					if(sb[l]==lb[0])
					{
						if(sb[l+1]!=lb[1] && sb[i+1]==lb[1])
						{
							c=i;
						}
						else
						{
							c=l;
						}
					}
				}

				if(c>0)
				{
					for(l=c;l<len;l++)
					{
						if(sb[l]==lb[j])
							j++;
					}
				}
				else
				{
					for(l=0;l<eleman2;l++)
					{
						if(sb[l]==lb[j])
							j++;
					}
				}
			}
			else
			{
				i++;
			}
		}
		//cout<<"\n<"<<c<<","<<j<<","<<lb[j]<<">";
		printf("\n<%d,%d,%c>",c,j,lb[j]);
		if(k==7)
		{
			for(i=0;i<7;i++)
			{
				if(eleman2>j)
				{
					sb[i]=sb[j+i+1];
				}
				else
					sb[i]=sb[j+i];

			}
			if(eleman2>j)
			{
				for(i=0;i<j+1;i++)
				{
					sb[k-j-1]=lb[i];
					k++;
				}
			}
			else
			{
				for(i=0;i<j+1;i++)
				{
					sb[k-j]=lb[i];
					k++;
				}
			}
		}
		else
		{
			for(i=0;i<j+1;i++)
			{
				sb[k]=lb[i];
				k++;
			}
		}
		for(i=0;i<(4-j);i++)
		{
			lb[i]=lb[i+1+j];
		}
		for(l=0;l<j+1;l++)
		{
			lb[i]=str[l];
			i++;
		}
		if(str!='\0')
		{
			for(l=0;l<13;l++)
			{
				str[l]=str[l+1+j];
			}
			for(l=13;l>(13-j);l--)
			{
				str[i]='\0';
			}

		}
		printf("\n In sb:%s",sb);//cout<<"\n In sb: "<<sb;
		printf("\t\t In lb:%s",lb);//<<"\t\t In lb: "<<lb;
		printf("\t\t In str:%s",str);//cout<<"\t\t In str: "<<str;

	}
	getch();

    return 0;
}
	/*char str[13]="abracadabra";
	char sb[10]="",lb[10]="";
	int i,j,c=0,k=0,m,l,len,flag;
	//system("cls");
	int eleman=(sizeof sb/sizeof*sb);
	int eleman2=(sizeof lb/sizeof*lb);
	printf("%d\n",eleman);
	strcpy(str,"abracadabra");
	//cout<<"Given string is: "<<str;

	printf("\nGiven String is:%s",str);


	for(i=0;i<5;i++)
	{
		lb[i]=str[i];
	}
	for(i=0;i<11;i++)
	{
		str[i]=str[i+5];
	}

	for(m=0;m<6;m++)
	{
		i=0;
		j=0;
		flag=0;
		len=strlen(sb);
		//cout<<"\nlength of sb : "<<len;
		printf("\nlength of sb:%d",sb);
		while(flag==0 && i<len)
		{
			if(sb[i]==lb[0])
			{
				flag=1;
				for(l=0;l<len;l++)
				{
					if(sb[l]==lb[0])
					{
						if(sb[l+1]!=lb[1] && sb[i+1]==lb[1])
						{
							c=i;
						}
						else
						{
							c=l;
						}
					}
				}

				if(c>0)
				{
					for(l=c;l<len;l++)
					{
						if(sb[l]==lb[j])
							j++;
					}
				}
				else
				{
					for(l=0;l<eleman2;l++)
					{
						if(sb[l]==lb[j])
							j++;
					}
				}
			}
			else
			{
				i++;
			}
		}
		//cout<<"\n<"<<c<<","<<j<<","<<lb[j]<<">";
		printf("\n<%d,%d,%c>",c,j,lb[j]);
		if(k==7)
		{
			for(i=0;i<7;i++)
			{
				if(eleman2>j)
				{
					sb[i]=sb[j+i+1];
				}
				else
					sb[i]=sb[j+i];

			}
			if(eleman2>j)
			{
				for(i=0;i<j+1;i++)
				{
					sb[k-j-1]=lb[i];
					k++;
				}
			}
			else
			{
				for(i=0;i<j+1;i++)
				{
					sb[k-j]=lb[i];
					k++;
				}
			}
		}
		else
		{
			for(i=0;i<j+1;i++)
			{
				sb[k]=lb[i];
				k++;
			}
		}
		for(i=0;i<(4-j);i++)
		{
			lb[i]=lb[i+1+j];
		}
		for(l=0;l<j+1;l++)
		{
			lb[i]=str[l];
			i++;
		}
		if(str!='\0')
		{
			for(l=0;l<13;l++)
			{
				str[l]=str[l+1+j];
			}
			for(l=13;l>(13-j);l--)
			{
				str[i]='\0';
			}

		}
		printf("\n In sb:%s",sb);//cout<<"\n In sb: "<<sb;
		printf("\t\t In lb:%s",lb);//<<"\t\t In lb: "<<lb;
		printf("\t\t\ In str:%s",str);//cout<<"\t\t In str: "<<str;

	}
	getch();

    return 0;
}*/
