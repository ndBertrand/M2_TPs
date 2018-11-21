#include<stdio.h>
#include<netinet/ip.h>
#include<netinet/ip_icmp.h>
#include <sys/socket.h>
#include <arpa/inet.h>



void display(unsigned short *buffer, int length){
	int count = 0;
	int i;
	for(i = 0; i< length ; i = i+2){
	    if(count%8 == 0)
		printf("\n");
	    printf("%02x%02x ", (unsigned char)buffer[i], (unsigned char)buffer[i+1]);
	    count++;
	}
}

struct iphdr remplir_iphdr(unsigned short ihl,unsigned short tot_len, unsigned short protocol, unsigned short version,unsigned short tos, unsigned short id,unsigned short frag_off,unsigned short ttl,unsigned short check,char *src_addr, char *dst_addr){
	
	struct iphdr myiphdr;

	myiphdr.ihl = ihl;
	myiphdr.tot_len = tot_len;	
	myiphdr.protocol = protocol;
	myiphdr.version = version;

	myiphdr.tos = tos;
	myiphdr.id = id;
	myiphdr.frag_off = frag_off ;
	myiphdr.ttl = ttl ;
	myiphdr.check = check;
	myiphdr.saddr= inet_addr(src_addr);
	myiphdr.daddr= inet_addr(dst_addr);

	return myiphdr;
}

struct icmphdr remplir_icmphdr(unsigned short icmp_echo,unsigned short code,unsigned short checksum, unsigned short id,unsigned short sequence){

	struct icmphdr myicmp;

	myicmp.type = icmp_echo;		
	myicmp.code = code;		
	myicmp.checksum = checksum;
	myicmp.un.echo.id = id;
	myicmp.un.echo.sequence = sequence ;

	return myicmp;
}

void afficherICMP(struct icmphdr myicmp){
	printf("%d\n, %d\n, %d\n, %d\n, %d\n",
	myicmp.type,		
	myicmp.code,		
	myicmp.checksum,
	myicmp.un.echo.id,
	myicmp.un.echo.sequence);
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
	unsigned short tab[length];
	/* int i;
	for(i=0; i < length ; i++){
		tab[i] = i;
	}*/
	//display(tab,length);

	
	unsigned short icmp_echo = ICMP_ECHO; // ECHO REQUEST		
	unsigned short code = 0;		
	unsigned short checkSum = 0;
	unsigned short id_icmp = 5678;
	unsigned short sequence = 1;

	short icmpTab[5] = {icmp_echo,code,checkSum,id_icmp,sequence};
	struct icmphdr myicmp = remplir_icmphdr(icmp_echo,code,checkSum,id_icmp,sequence);
	
	/*display(icmpTab,5);

	afficherICMP(myicmp);
	*/

	
	
	unsigned short ihl = sizeof(struct iphdr) / 4;;
	unsigned short tot_len =  sizeof(struct iphdr);
	unsigned short protocol = IPPROTO_ICMP;
	unsigned short version = 4;

	unsigned short  tos = 4;
	unsigned short  id_ip= 1234;
	unsigned short  frag_off=7;
	unsigned short  ttl=100;
	
	unsigned short check = 0;
	char *saddr="192.168.0.1";
	char *daddr="192.168.0.2" ;

	unsigned short ipTab[15] = {ihl,tot_len,protocol,version,tos,tot_len,id_ip,frag_off,ttl,check,icmp_echo,code,checkSum,id_icmp,sequence};
	struct iphdr myiphdr =   remplir_iphdr(ihl,tot_len,protocol,version,tos,id_ip,frag_off,ttl,check,saddr,daddr);

	display(ipTab,15);


	printf("\n\nCALCUL DES CKECKSUM\n");

	//----------CALCUL DES CHECKSUM----------------------
	myicmp.checksum = checksum(icmpTab,5);
	printf("\n mon nouveau checksum du ICMP %hu \n", myicmp.checksum);

	myiphdr.check = checksum(ipTab,9);
	printf("\n mon nouveau checksum du IP %hu  \n", myiphdr.check);
		
	
	return 0;
}
