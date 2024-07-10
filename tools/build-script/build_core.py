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

importlib.reload(sys)

DEBUG = True


from func import prepare_folder
from func import get_args
from func import copy_dir
from func import exec_cmd
from func import merge
from build_env import ZIPEXE

if __name__ == '__main__':
    args = get_args()
#save cwd
    old_cwd = os.path.abspath(os.getcwd())
    if(DEBUG):
        print(f'invoker:{old_cwd}')

    script_path = os.path.abspath(os.path.split(os.path.realpath(__file__))[0])
    if(not os.path.exists(os.path.join(script_path, f'{args.branch}.py'))):
        print(f'branch {args.branch} not exist. please check tools/build-script folder.')
        sys.exit(404)
    entity = importlib.import_module(args.branch)

    os.chdir(os.path.join(script_path, os.path.pardir, os.path.pardir))
    cur_cwd = os.path.abspath(os.getcwd())
    if(DEBUG):
        print(f'cur_cwd:{cur_cwd}')

    val = 0

    if( args.type == 'full_code'): #build pac and simulator
        simulator = False
        if(args.simulator == 'true'): #simulator
            val = entity.build(project = args.project, modules = [], simulator = True, mode = 'clean', log_file = args.log_name)
        else: #module or full build
            val = prepare_folder(args.output, remove=True)
            val = entity.build(project = args.project, modules = args.modules, simulator = False, mode = args.mode, log_file = args.log_name)
            if(val == 0):
                pac_dir = entity.get_pac_dir(cur_cwd, args.project)
                if(DEBUG):
                    print(f'pac_dir:{pac_dir}')
                copy_dir(pac_dir, args.output)

    elif args.type == 'idh_code_zip': #make idh zip
        val = prepare_folder(args.output)
        if(DEBUG):
            print(f'prepare_folder return {val} {str(sys._getframe().f_lineno)}')
        if(val == 0):
            val = entity.idh(args.branch, args.project, args.log_name)
            idh_root = entity.get_idh_root(cur_cwd)
            if(DEBUG):
                print(f'idh_root:{idh_root}')
            #zip files
            if(val == 0):
                folders = os.listdir(idh_root)
                for folder_name in folders:
                    folder = os.path.join(idh_root, folder_name)
                    if(os.path.isdir(folder)):
                        zip = os.path.join(idh_root, folder_name+'.zip')
                        cmd = f'\"{ZIPEXE}\" a -tzip {zip} {folder} -mx0'
                        val = exec_cmd(cmd)
                        if(val != 0):
                            break
                        #copy zip
                        try:
                            if(DEBUG):
                                print(f'SRC:{zip}, DEST:{args.output}')
                            shutil.copy(zip, args.output)
                        except:
                            print(f"file:{__file__} Line:{str(sys._getframe().f_lineno)} Unexpected error:{sys.exc_info()}")
                            val = -1

    elif args.type == 'idh_code': #idh zip verify
        val = prepare_folder(args.output, remove=True)
        #extract files
        idh_root = entity.get_idh_root(cur_cwd)
        idh_verify = entity.get_idh_verify_dir(cur_cwd)
        idh_src = os.path.join(idh_verify, entity.IDH_CODE)
        if(DEBUG):
            print(f'idh_verify:{idh_verify}')
        val = prepare_folder(idh_verify)
        idh_code_zip = os.path.join(idh_root, entity.IDH_CODE+'.zip')
        idh_project_zip = os.path.join(idh_root, args.project +'.zip')
        if(val == 0):
            #extract files.
            cmd = f'\"{ZIPEXE}\" x -y {idh_code_zip} -o{idh_verify}'
            val = exec_cmd(cmd)
            if(val == 0):
                cmd = f'\"{ZIPEXE}\" x -y {idh_project_zip} -o{idh_verify}'
                val = exec_cmd(cmd)
                if(val == 0):
                    #need copy from idh_verify\project to idh_verify\IDH_CODE
                    merge(os.path.join(idh_verify, args.project), idh_src)
                    os.chdir(idh_src)
                    val = entity.build(project = args.project, modules = [], simulator = False, mode = 'new', log_file = args.log_name)
                    if(val == 0):
                        pac_dir = entity.get_pac_dir(idh_src, args.project)
                        if(DEBUG):
                            print(f'pac_dir:{pac_dir}')
                        copy_dir(pac_dir, args.output)

    os.chdir(old_cwd)
    print(os.getcwd())
    sys.exit(val)

