#include<stdio.h>
#include<netinet/ip.h>
#include<netinet/ip_icmp.h>
#include <sys/socket.h>
#include <arpa/inet.h>



void display(char *buffer, int length){
	int i;
	for(i=0; i < length ; i++){
		buffer[i] = i;
	}
	
	int count = 0;
	for(i = 0; i< length ; i = i+2){
	    if(count%8 == 0)
		printf("\n");
	    printf("%02x%02x ", (unsigned char)buffer[i], (unsigned char)buffer[i+1]);
	    count++;
	}
}

struct iphdr remplir_iphdr(int tos, int id,int frag_off,int ttl,char *src_addr, char *dst_addr){
	
	struct iphdr myiphdr;
	myiphdr.ihl = sizeof(struct iphdr) / 4;
	myiphdr.version = 4;
	myiphdr.tos = 0;
	myiphdr.tot_len =  sizeof(struct iphdr) ;
	myiphdr.id = id;
	myiphdr.frag_off = frag_off ;
	myiphdr.ttl = ttl ;
	myiphdr.protocol = IPPROTO_ICMP;;
	myiphdr.check = 0;
	myiphdr.saddr= inet_addr(src_addr);
	myiphdr.daddr= inet_addr(dst_addr);

	return myiphdr;
}

struct icmphdr remplir_icmphdr(unsigned short icmp_echo,unsigned short code,unsigned short checksum, unsigned short id,unsigned short sequence){

	struct icmphdr myicmp;

	myicmp.type = ICMP_ECHO;		
	myicmp.code = 0;		
	myicmp.checksum = 0;
	myicmp.un.echo.id = id;
	myicmp.un.echo.sequence = sequence ;

	return myicmp;
}

unsigned short checksum(unsigned short * addr,int count){

           /* Compute Internet Checksum for "count" bytes
            *         beginning at location "addr".
            */
       register long sum = 0;
	register u_short *w = addr;
	u_short checksum = 0;

        while( count > 1 )  {
           /*  This is the inner loop */
               sum += *w++;
               count -= 2;
       }

           /*  Add left-over byte, if any */
       if( count > 0 )
               sum += * (unsigned char *) addr;

           /*  Fold 32-bit sum to 16 bits */
       while (sum>>16)
           sum = (sum & 0xffff) + (sum >> 16);

       checksum = ~sum;

	return (checksum);
}



int main(){

	int length = 50;
	char tab[length];
	/*display(tab,length);*/


	unsigned short icmp_echo = ICMP_ECHO;		
	unsigned short code = 0;		
	unsigned short checkSum = 0;
	unsigned short id = 5;
	unsigned short sequence = 10 ;

	short icmpTab[5] = {icmp_echo,code,checkSum,id,sequence};
	struct icmphdr myicmp = remplir_icmphdr(icmp_echo,code,checkSum,id,sequence);
	myicmp.checksum = checksum(icmpTab,5);

	printf("mon nouveau checksum %hu \n", myicmp.checksum);
	
	
	/*iphdr remplir_iphdr(int tos, int id,int frag_off,int ttl,char *src_addr, char *dst_addr)
	struct iphdr myiphdr = remplir_iphdr(5,3,4,8,"192.168.0.1","192.168.0.2");*/
	
	
	
	return 0;
}
