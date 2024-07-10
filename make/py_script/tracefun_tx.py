
# translate function 'SCI_TRACE_LOW' into 'TRACE_FUNCTION_L1'
# we will use different function name start with 'TRACE_FUNCTION_L1'
# according to the argv in the orginal function 'SCI_TRACE_LOW' .
# for e.g., 		SCI_TRACE_LOW("Server: Delete the server(%d) immediately!\n", /
#  server_ptr->INFO.server_info.server_ID);
# will be translated into 'TRACE_FUNCTION_L1_P2("Server: Delete the server(%d) /
#  immediately!\n", server_ptr->INFO.server_info.server_ID)'

# model assumption
# expression = invalid_part : sci_func_name : invalid_part: '(' :  invalid_part: '"' :@:'"': invalid_part: ',' :argv:  ','|')':invalid_part: ';' invalid_part 
# invalid_part = 	format_c | comment_str 
# argv = invalid_part:valid_name_char:argv
# valid_name_char = alpha|'_'|digital

import sys
import os

symbal_table = ['(','/']
format_table=['\t','\n',' ','\r','\\']
#terminate_invalid_table = [digit, alpha, '(', ')', '"', ',', '_', ';']
#terminate_argv_table = [',',')']
#terminate_func_name_table=[';', ',', ')']
prefix_buf=[]

sym_buf=[] 
sci_func_name = 'SCI_TRACE_LOW'
mp_func_name_prefix = 'TRACE_FUNCTION_L1'
#path=r'D:\mp\adapt2threadx\python\sci_mem.c' 
src_file=r'D:\mp\adapt2threadx\python\New0001.c'
#src_file=r'D:\mp\adapt2threadx\python\test\layer1_sbc.c'

#src_file=r'D:\mp\adapt2threadx\python\threadx_port\BootLoader\new bootloader-big page\stage1\src\usb2_m8120.c' 
out_list=[]
line=''
file_buf = []
out_file = []
func_name_list=['SCI_TRACE_LOW', 'PHY_TRACE', 'LAYER1_TRACE_INF', 'PHY_TRACE_CBCH', 'LAYER1_MP3SBC_TRACE', 'LAYER1_EDGE_TRACE', 'SCI_TRACE_AUD', 'PHY_LAYER1_ERR', 'SCI_TRACE', 'DL_TRACE', 'SCI_MAC_TRACE', 'RLC_TRACE_DL', 'RLC_TRACE_PDU', 'RLC_TRACE', 'SCI_RR_TRACE', 'SCI_RRA_TRACE', 'TPC', 'TPI', 'TPCI', 'TPE', 'TPH', 'TPL', 'TPDF', 'TPD', 'TPS', 'TPERR']

def push_sym(a):
	sym_buf.append(a)

def pop_sym():
	return sym_buf.pop(0)

def get_sym():
	a=sym_buf.pop(0)
	sym_buf.insert(0,a)
	return a 

def count_sym():
	return len(sym_buf)

def get_new_line(l,f,i,seg_record):
	global out_list
#	line=file_buf[l]
#	if (i<len(line)):
#		seg_record.append(file_buf[l][i])
	if	len(file_buf[l])<=(i):
		l=l+1
		i=0
		if (l>=len(file_buf)) or (len(file_buf[l])<=0):
			return (l,-1)
		else:
#			print out_list
#			out_list.append(file_buf[l])
			return (l,i)
	return (l,i)

def error_exit(f,path,l,sci_func_name):
	print 'error in get symbol!'
	print path
	print l
	print sci_func_name
	f.close()
	sys.exit()

# this function ignore blanks and comments, until it get a code or end unexpectedly.
# assume current char is not recorded, and will be recorded until it has been handled

def get_next_sym(l, f,i,seg_record):
	line=file_buf[l]
	while True:
		# skip ' ','\t','/*...*/',del endline symbol such as '\n'
		#current char is not recorded
		while  (i<len(line)) and (line[i] in format_table) :

			if (line[i]!='\n') and (line[i]!='\\'):
				#ignore \n and \\
				seg_record.append(line[i])
			i=i+1
			
		if (len(line)<=(i)):
			(l,i)=get_new_line(l,f,i,seg_record)
			if (-1==i):
				return (l, -1)
			else:
				line=file_buf[l]
				continue
#		seg_record.append(line[i])
		comment=True
		while comment:
			if	 (line[i]=='/'):
				seg_record.append(line[i])
				i=i+1
				(l,i)=get_new_line(l,f,i,seg_record)
				if (-1==i):
					return (l,-1)
				line=file_buf[l]
				if (line[i]=='*'):
					seg_record.append(line[i])
					i=i+1

					(l,i)=get_new_line(l,f,i,seg_record)
					if (-1==i):
						return (l,-1)
					line=file_buf[l]

					comment_unmatch=True
					while comment_unmatch:
					# expect '*'
						if (line[i]=='*'):
							seg_record.append(line[i])
							i=i+1
							(l,i)=get_new_line(l,f,i,seg_record)
							if (-1==i):
								return (l,-1)
							line=file_buf[l]

							if (line[i]=='/'):
								comment_unmatch=False
								comment =False
							else:
								continue

							

						seg_record.append(line[i])
						i=i+1
						(l,i)=get_new_line(l,f,i,seg_record)
						if (-1==i):
							return (l,-1)
						line=file_buf[l]

				elif (line[i]=='/'):
					# ignore the left of the line, jump to next line
					seg_record.pop()
					i=0
					l=l+1
					(l,i)=get_new_line(l,f,i,seg_record)
					if (-1==i):
						return (l,-1)
					line=file_buf[l]
				else:
					return (l,i)
			else:
				return (l,i)

def get_next_argv(l,f,i,seg_record):
	line=file_buf[l]
	sym_buf2=[]
	parse_argv=True
	while parse_argv:
	
#		seg_record.append(line[i])
		(l,i) = get_next_sym(l,f,i,seg_record)
		if (i==-1):
			return (l,-1)
		line=file_buf[l]
		if line[i]=='(':
			sym_buf2.append(line[i])
		elif (line[i]==')'):
			if (len(sym_buf2)<=0):
			# may be the end of this argv, we don't consider the grammer here
				return (l,i)
			else:
				sym_buf2.pop()
			
		elif line[i]==',':
			if (len(sym_buf2)==0):
#end of argv
				return (l,i)
#else
				# we don't analyze the grammer , we assume it's right.
				# so we think it on the half way. e.g. func(a,b), just skip it

		seg_record.append(line[i])
		i=i+1
		(l,i)=get_new_line(l,f,i,seg_record)
		if (-1==i):
			return (l,-1)
		line=file_buf[l]

		   

def 	translate_sci_trace_func(nu_argv,out_list):
	if (nu_argv==1):
		func_name='TRACE_FUNCTION_L1'
	elif  (nu_argv==2):
		func_name='TRACE_FUNCTION_L1_P1'
	elif  (nu_argv==3):
		func_name='TRACE_FUNCTION_L1_P2'
	elif  (nu_argv==4):
		func_name='TRACE_FUNCTION_L1_P3'
	elif  (nu_argv==5):
		func_name='TRACE_FUNCTION_L1_P4'
	else  :
		return -1

	out_list.insert(1,func_name)

def integrate(out_list,out_file):
	src_list=out_list[2:]

	des_list=out_list[:2]
	for n in src_list:
		# ignore '\r','\n','\' in the end of the line
		end_pos=len(n)
		if (end_pos)>0:
			j=n[end_pos-1]

			if (j=='\n'):
				end_pos=end_pos-1
				if (end_pos)>0:
					j=n[end_pos-1]
			if (j=='\r'):
				end_pos=end_pos-1
				if (end_pos)>0:
					j=n[end_pos-1]
			if (j=='\\'):
				end_pos=end_pos-1
			start_pos=0

		while (start_pos<len(n)) and (n[start_pos] in ['\t',' ']):
			start_pos=start_pos+1
		l= n[start_pos:end_pos]
		des_list.append(l)

	des_list.append('\n')
	l= ''.join(des_list)
	out_file.append(l)

def translate_sci_func_extra(out_list,out_file):

# divide the paramenters into several parts to use mp trace function
	divide_nu= 4
	out_list2=out_list[:1]
	line=out_list[1]
#get '%' which is used to describe format of the parameter

	line_list=list(line)
	new_seg=[]
	start=0
	end=0
	para_list=[]
	k=0
	while k<len(line_list):
		print line[k]
		if (line[k]=='%'):
			if not (((k+1)<len(line_list) and (line[k+1]=='%'))):
			#start a parameter
				new_seg = line_list[start:k]
				m=''.join(new_seg)
				para_list.append(m)
				start=k
		k=k+1
	new_seg = line_list[start:k]
	m=''.join(new_seg)
	para_list.append(m)	

# each 4 para distribute to a func
	start = 0
	end=4
	start_para =2
	end_para=0
	while (start<len(para_list)):
		end=start+4

		if end>len(para_list):
			end=len(para_list)
	#		end_para = len(out_list)
#		else:
#			if ( '%' in para_list[start]):
#				end_para = start_para+4
#			else:
#				end_para = start_para+3
				
		out_list2 = para_list[start:end]
		nv_argv=1
		for strtemp in out_list2:
			if ( '%' in strtemp):
				nv_argv =nv_argv+1
		end_para=start_para+nv_argv-1
		is_end_para = (len(para_list)==end)
		is_end_out = (len(out_list)==end_para)
		if (is_end_para) and not(is_end_out):
			#return -1
			end_para = len(out_list)
		if (not(is_end_para)) and (is_end_out):
			return -1
		out_list2.insert(0,out_list[0])
		if (start>0):
			out_list2.insert(1,'("')
		if (end<len(para_list)):
			out_list2.append('",')
		for line in out_list[start_para:end_para]:
			out_list2.append(line)
		if (end_para<len(out_list)):
			line = out_list2[len(out_list2)-1]
			d=list(line)
			sp=len(d)-1
			while (sp>0) and (d[sp] in [' ','\t']):
				sp=sp-1

			if (d[sp]=='\n'):
				del d[sp]
			sp = len(d)-1
			if (sp>=0):
				if (','==d[sp]):
					del d[sp]
			line = ''.join(d)
			out_list2.pop()
			out_list2.append(line)
			out_list2.append(');')
			
		if (-1==translate_sci_trace_func(nv_argv,out_list2)):
			return -1
		else:
			integrate(out_list2,out_file)

		start=end
		start_para=end_para

def 		translate_multi_sym_one_file(path):
	global func_name_list
	
	for func_sym in func_name_list[:]:
		print func_sym
		translate_one_file(func_sym,path)
	
#def	translate_one_file(path):
def 	translate_one_file(sci_func_name,path):
	global out_list
	global out_file
	global file_buf
	global sym_buf
	
	out_list=[]
	line=''
	file_buf = []
	out_file = []
	change=False
	sym_buf=[]
	
	f=file(path,'rU')
	while True:
		line =f.readline()
		if (len(line)<=0):
			f.close()
			break
		file_buf.append(line)

	line_end=f.newlines
	f.close()
	
	l=0

	while True:
		#start_seg=0
		#end_seg=0
		#we devide the command line according to each parameter  into the out_list

		seg_record=[]
		search_next=False
		if (l>=len(file_buf)):
			# finished searching , write back
			f.close()
			if (change==True):
				f=file(path,'w')
				for line in out_file:
					f.write(line)
				f.close()
			return 0
		line=file_buf[l]
#		if l==192:
#			print l
		sym_buf=[]
		#ignore those are definition or other names
		if	(not (sci_func_name in line)) or ('#define' in line):
			out_file.append(line)
			l=l+1
			search_next=True
		else:
			sci_func_pos =line.rfind(sci_func_name) 
			i=sci_func_pos+len(sci_func_name)
			#start_seg=i
			# only match the name, not those contain the name
			c=line[i]
			if (c.isalpha() or c.isdigit() or (c =='_')):
				out_file.append(line)
				l=l+1				
				search_next=True
			else:
				if (sci_func_pos>0):
					prefix = 0
					currentl=l
					(l,prefix) = get_next_sym(l, f,prefix,seg_record)
					if (prefix==-1) or \
					   (currentl!=l) or \
					   ((prefix<sci_func_pos) and (line[prefix]!='{')):
						if (currentl!=l):
							# case of '/*.....*/'
							l=currentl
							
						out_file.append(line)
						l=l+1				
						search_next=True

			if search_next:
				continue
				
# split the string into 2 part, before this sci func name and after this sci func name

			split = []
			split = line.split(sci_func_name)
			#get the first part before the name
			out_list = split[0:1]
			# extract '{' from the line
#			line = split[0]
#			for i in range(0,len(line)):
#				if (line[i]=='{'):
#					prefix_buf.append(line[i])

			nu_argv=0;

			# we analyze the left after the name
			seg_record=[]
			paretheses=True
			while paretheses:
				(l,i) = get_next_sym(l, f,i,seg_record)
				if i==-1:
					error_exit(f,path,l,sci_func_name)
				else:
					line=file_buf[l]
				
				b=line[i]
				if b=='(' :
					push_sym(line[i])
					seg_record.append(line[i])
					i=i+1
				else:
					if (count_sym()==0):
						out_file.append(line)
						l=l+1
						search_next=True
						break
						#error_exit(f,path,l,sci_func_name)
					else:
						paretheses=False

						
# expect '"'
			if search_next:
				continue
			if (line[i]=='"')	:

				expect_p=True
				while expect_p:
					seg_record.append(line[i])
					i=i+1
					(l,i)=get_new_line(l,f,i,seg_record)
					if (-1==i):
						error_exit(f,path,l,sci_func_name)
					line=file_buf[l]

# expect '"'
					if (line[i]=='"'):
						expect_p=False
						nu_argv=1
			else:
				#not a function call, ignore this line
				out_file.append(line)
				l=l+1
				continue					
				#error_exit(f,path,l,sci_func_name)

			seg_record.append(line[i])
			i=i+1
#			(l,i)=get_new_line(l,f,i,seg_record)
			(l,i)=get_next_sym(l,f,i,seg_record)
			if (-1==i):
				error_exit(f,path,l,sci_func_name)
			else:
				line=file_buf[l]


# while (!EOF(file))

#	expect ',' or ')'
			next_argv=True;
			while next_argv:
#	  if ',',
				if (line[i]==','):
					nu_argv=nu_argv+1
					seg_record.append(line[i])
					i=i+1
					(l,i)=get_new_line(l,f,i,seg_record)
					if (-1==i):
						error_exit(f,path,l,sci_func_name)
					else:
						line=file_buf[l]

					#write the seg into out_list
					seg_str=''.join(seg_record)
					out_list.append(seg_str)
					seg_record=[]
					#seg_record.append(line[i])
					(l,i) = get_next_argv(l,f,i,seg_record)
					if (i==-1):
						error_exit(f,path,l,sci_func_name)
					else:
						line=file_buf[l]

				elif (line[i]==')'):
					match_paretheses=True
					while match_paretheses:
						if (line[i]==')'):
							if (count_sym()<=0):
								error_exit(f,path,l,sci_func_name)
							if (pop_sym()!='('):
								error_exit(f,path,l,sci_func_name)
							seg_record.append(line[i])
							i=i+1
							(l,i)=get_next_sym(l,f,i,seg_record)
							if (-1==i):
								error_exit(f,path,l,sci_func_name)
							else:
								line=file_buf[l]

						else:
#	  if ')' , expect ';', end
							match_paretheses=False
							if (line[i]==';') and (count_sym()==0):
								seg_record.append(line[i])
								#write the seg into out_list
								seg_str=''.join(seg_record)
								out_list.append(seg_str)
								seg_str=''
								if (i <(len(line)-1)):
									seg_str = line[i+1:len(line)]

								if (nu_argv>4):
									if (-1==translate_sci_func_extra(out_list,out_file)):
										print 'error in translate'
										print nu_argv
										print path
										print l
										print out_list
									
									change=True
								else:
									if (-1==translate_sci_trace_func(nu_argv,out_list)):
										print 'error in translate'
										print nu_argv
										print path
										print l
										print out_list

									else:
										integrate(out_list,out_file)
										change=True
								if len(seg_str):
									out_file.append(seg_str)
									seg_str=''
								translate_one_finished=True
								next_argv=False;
								l=l+1
							else:
								error_exit(f,path,l,sci_func_name)
				else:
					error_exit(f,path,l,sci_func_name)

#translate_one_file(src_file)

def collect_func_name(path):
	global out_list
	global out_file
	global file_buf
	global func_name_list
	out_list=[]
	line=''
	file_buf = []
	out_file = []
	f=file(path,'rU')
	while True:
		line =f.readline()
		if (len(line)<=0):
			f.close()
			break
		file_buf.append(line)
	line_end=f.newlines
	f.close()
	l=0
	while True:
		if (l>=len(file_buf)):
# finished searching , write back
			f.close()
			return 0
		line=file_buf[l]
		if	(sci_func_name in line):
			if ('#define' in line):
				#print line
				if ('//' in line):
					l=l+1
					continue
				start_sym = line.rfind('#define')+len('#define')
				end_sym=start_sym
				sym_name=''
				while (start_sym<len(line)):
					if (line[start_sym] in ['\t',' ']):
						start_sym=start_sym+1
					else:
						break
				if (start_sym<len(line)):
					end_sym=start_sym
					while (end_sym<len(line)):
						if (not (line[end_sym] in ['\t',' ','('])):
							end_sym=end_sym+1
						else:
							break
					sym_name=line[start_sym:end_sym]
					if not(sym_name ==''):
						if not (sym_name in func_name_list):
							func_name_list.append(sym_name)
							print line

		l=l+1


def translate_dir(path):
	filemap = []
	global func_name_list
	
	for dir, folders, files in os.walk(path):
		if not '.svn' in dir:
			for name in files:
				if (('.c') in name) or ('.h' in name) or ('.inc' in name):
					filemap.append(os.path.join(dir, name))

	for  file in filemap:
		collect_func_name(file)

	print func_name_list

	filemap = []
	for dir, folders, files in os.walk(path):
		if not '.svn' in dir:
			for name in files:
				if ('.c') in name :
					filemap.append(os.path.join(dir, name))
	for  file in filemap:
		print file
		translate_multi_sym_one_file(file)
		#translate_one_file(sci_func_name,src_file)


	

#def main(argv):
if (len(sys.argv)<2):
	print 'Usage: Please input the file path you want to change'
	print 'e.g. python tracefun.py D:\\mp\\adapt2threadx\\python\\test\\'
	#print 'invalid path'
	sys.exit()
else:
	project_path = sys.argv[1]
if not(os.path.exists(project_path)):
	print 'path doesn\'t exist'
	sys.exit()


translate_dir(project_path)
#translate_multi_sym_one_file(src_file)
#translate_one_file(sci_func_name,src_file)

