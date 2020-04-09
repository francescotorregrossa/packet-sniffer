#Packet sniffer in Python
#Linux based implementation

'''
import socket

#create an INET raw socket
s = socket.socket(socket.AF_INET, socket.SOCK_RAW, socket.IPPROTO_TCP)

#receive a packet
while True:
	print s.recvfrom(65565)

'''
import socket, sys
from struct import *

#Get string of 6 characters as ethernet address into dash seperated hex string
def eth_addr(a):
	b = "%.2x:%.2x:%.2x:%.2x:%.2x:%.2x" %(ord(a[0]),ord(a[1]),ord(a[2]),ord(a[3]),ord(a[4]),ord(a[5]))
	return b

#create an INET, STREAMing socket
try:
	#s=socket.socket(socket.AF_PACKET,socket.SOCK_RAW,socket.ntohs(0x0003))
	s=socket.socket(socket.AF_INET, socket.SOCK_RAW, socket.IPPROTO_TCP)
	#AF_INET is the family of sockets created - TCP or UDP
	#Socket type is SOCK_RAW instead of SOCK_STREAM or SOCK_DGRAM
	#Socket protocol specified is IP-PROTO_<TCP/UDP/ICMP>
except socket.error,msg:
	print 'Socket could not be created. Error Code : '+str(msg[0])+'Message '+msg[1] 
	sys.exit
count = 0
print 'Getting a packet\n\n'
#get a packet 
while True:
	packet = s.recvfrom(65565) #keep in mind that this port binding won't work in Windows
				   #Windows uses a Winsock API hook or Winpcap driver for sockets
	#socket.recvfrom(buffersize,[flags]) gets the data from the socket. O/P - (string,address)

	print 'Packet Received:'+str(packet)+'\n\n'
	count= count+1
	#packet string from tuple
	packet=packet[0]
	
	#-------------------L2 Information-------------------------------------
	eth_length = 14
	eth_header = packet[:eth_length]
	eth_unpack =  unpack('!6s6sH',eth_header)
	eth_protocol = socket.ntohs(eth_unpack[2])
	print '###############Layer 2 Information############'
	print 'Destination MAC:'+eth_addr(packet[0:6])
	print 'Source MAC:'+eth_addr(packet[6:12])
	print 'Protcol:'+str(eth_protocol)
	print '-----------------------------------------------------------------\n\n' 
	
        #-------------------IP HEADER EXTRACTION--------------------------------
	#take the first 20 characters for the IP header
	ip_header = packet[0:20]
	
	#now unpack 'em
	header_unpacked = unpack('!BBHHHBBH4s4s',ip_header)
	#https://docs.python.org/2/library/struct.html#format-characters
	
	version_ih1= header_unpacked[0] 
	version = version_ih1 >> 4 
	ih1 = version_ih1 & 0xF
	
	iph_length = ih1*4
	
	ip_total_length = header_unpacked[2]
	ttl = header_unpacked[5]
	protocol = header_unpacked[6]
	source_add = socket.inet_ntoa(header_unpacked[8])
	destination_add = socket.inet_ntoa(header_unpacked[9])
	print '##########IP Header Info##############'
	print 'TotalLength :' + str(ip_total_length) + '\nVersion : '+str(version)+ '\nIP Header Length:'+str(ih1)+'\nTTL:'+str(ttl)+'\nProtocol:'+str(protocol)+'\nSource Address:'+str(source_add)+'\nDestination Address:'+str(destination_add)
	print '-------------------------------------------\n\n'

	#-----------------------------------------------------------------------------

	#----------------TCP HEADER EXTRACTION----------------------------------------
	#tcp_header = packet[iph_length:iph_length+20] 
	#t=iph_length+eth_length
	tcp_header = packet[iph_length:iph_length+20]

	#unpack them 
	tcph = unpack('!HHLLBBHHH',tcp_header)
	
	source_port = tcph[0]
	dest_port = tcph[1]
	sequence = tcph[2]
	ack = tcph[3]
	resrve = tcph[4]
	tcph_len = resrve >> 4

	#print it all out
	print '###########TCP Header Info##############'
	print 'Source Port:'+str(source_port)
	print 'Destination Port:'+str(dest_port)
	print 'Sequence Number:'+str(sequence)
	print 'Acknowledgement:'+str(ack)	
	print 'TCP Header Length:'+str(tcph_len)
	print '------------------------------------------\n\n'
	#-------------------------------------------------------------------------------

	#------------------------Get the DATA-------------------------------------------
	h_size = iph_length+tcph_len*4
	data_size = len(packet)-h_size

	#get the data yo!
	data = packet[h_size:]
	print data
