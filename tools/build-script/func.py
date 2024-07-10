'''
********************************************************************************
** Copyright <2022>[-<2022>] Unisoc(Shanghai) Technologies Co.,Ltd             *
** Licensed under the Unisoc General Software License,                         *
** version 1.0 (the License);                                                  *
** you may not use this file except in compliance with the License.            *
** You may obtain a copy of the License at                                     *
** https://www.unisoc.com/en_us/license/UNISOC_GENERAL_LICENSE_V1.0-EN_US      *
** Software distributed under the License is distributed on an "AS IS" BASIS,  *
** WITHOUT WARRANTIES OF ANY KIND, either express or implied.                  *
** See the Unisoc General Software License, version 1.0 for more details.      *
********************************************************************************
'''

import argparse
import platform
import sys
import os
import json
import re
import importlib
import shutil
import stat

DEBUG = True


#check args
def get_args():
    arg_parser = argparse.ArgumentParser(description = "Mocor Build Script.")
    arg_parser.add_argument('--branch', '-b')
    arg_parser.add_argument('--project', '-p')
    arg_parser.add_argument('--simulator', default='false')
    arg_parser.add_argument('--mode', default='')
    arg_parser.add_argument('--type')
    arg_parser.add_argument('--log_name')
    arg_parser.add_argument('--output', '-o')
    arg_parser.add_argument('--modules', '-m', type=str, default=[], nargs='+')
    args = arg_parser.parse_args()
    args.output = re.sub("'", '', args.output)
    if(DEBUG):
        print(f'args.branch:{args.branch}')
        print(f'args.project:{args.project}')
        print(f'args.simulator:{args.simulator}')
        print(f'args.mode:{args.mode}')
        print(f'args.type:{args.type}')
        print(f'args.log_name:{args.log_name}')
        print(f'args.output:{args.output}')
        print(f'args.modules:{args.modules}')

    if(args.type == None or args.log_name == None or args.output == None or (args.type != 'idh_code_zip' and args.project == None )):
        print('Input parameters error.')
        sys.exit(1)
    return args


def copy_dir(src, dest):
    #copy files
    try:
        shutil.copytree(src, dest)
    except FileNotFoundError as efnf:
        print(str(efnf))
        return 11
    except Exception as e:
        print(str(e))
        return 255


def exec_cmd(cmd):
    val = 0
    if(DEBUG):
        print(f'Run {cmd}')
    val = os.system(cmd)
    if(DEBUG):
        print(f'Run {cmd} return {val}')
    return val


def prepare_folder(path, remove = False):
    val = 0
    if(not os.path.exists(path)):
        os.makedirs(path)
    else:
        #delete all files
        while(True):
            del_list = os.listdir(path)
            sys.stdout.flush()
            if(len(del_list) == 0):
                break
            for f in del_list:
                file_path = os.path.join(path, f)
                try:
                    if os.path.isfile(file_path):
                        os.remove(file_path)
                    elif os.path.isdir(file_path):
                        shutil.rmtree(file_path)
                except PermissionError as epe:
                    #print(str(epe))
                    #sys.stdout.flush()
                    #提取路径
                    splited = str(epe).split('\'')
                    #print(splited)
                    #sys.stdout.flush()
                    os.chmod(splited[1], stat.S_IWUSR)
                    try:
                        if os.path.isfile(splited[1]):
                            os.remove(splited[1])
                        elif os.path.isdir(splited[1]):
                            shutil.rmtree(splited[1])
                    except PermissionError as epe1:
                        print(str(epe1))
                        print('fail change mode and remove, exit')
                        sys.stdout.flush()
                        val = -1
    if(remove):
        os.rmdir(path)
    return val

def merge(src,dest): # 合并两个目录
    #print(f'merge {src} ==> {dest}')
    B_paths = os.listdir(dest) # 获取当前B中的目录结构
    for fp in os.listdir(src): # 遍历当前A目录中的文件或文件夹
        A_new_path = os.path.join(src,fp) # A中的文件或目录
        B_new_path = os.path.join(dest,fp) # B中对应的文件或路径，不一定存在
        if os.path.isdir(A_new_path): # A中的目录
            if os.path.exists(B_new_path): # 如果在B中存在
                merge(A_new_path,B_new_path) # 继续合并下一级目录
            else: # 如果在B中不存在
                shutil.copytree(A_new_path,B_new_path) # 完全复制目录到B
        elif os.path.isfile(A_new_path): # A中的文件
            shutil.copy2(A_new_path,B_new_path)