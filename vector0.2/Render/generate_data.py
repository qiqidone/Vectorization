#!/usr/bin/env python
#encoding=utf-8

height = 512
width = 512

def generate_data():
	file = open('data.txt','w');
	file.write('# v id x y\n')	
	for n in range(10):
		file.write('v '+str(n+1)+' '+str(n**2+n+100)+' '+str(n*3+n*31)+'\n')

	file.write('# l id ids ide cl cl cl cl cr cr cr cr\n')
	for n in range(10):
		file.write('l '+str(n+1)+' '+str(1)+' '+str(n+1)\
				   +' 10'*4+' 21'*4+'\n')
	file.close()

if __name__ == '__main__':
	generate_data()
		
