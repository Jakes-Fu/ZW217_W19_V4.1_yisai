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
import shutil
import platform
import sys
import importlib
import os
import json
import re
from pathlib import Path

importlib.reload(sys)

DEBUG = True

from build_env import VS2010EXE
from build_env import PERLEXE
from build_env import ZIPEXE

from func import prepare_folder
from func import copy_dir
from func import exec_cmd


SIMULATOR = 'MSDev2010.sln'

IDH_ROOT = 'IDH'
IDH_CODE = 'idh_code'
IDH_VERIFY = 'VERIFY'

BUILD_OUTPUT = 'build'
BUILD_OUTPUT_SUFIX = '_builddir'
IMG = 'img'


def get_build_dir(code_root, project):
    return os.path.join(code_root, BUILD_OUTPUT, project+BUILD_OUTPUT_SUFIX)


def get_simulator_dir(code_root, project):
    return os.path.join(code_root, BUILD_OUTPUT, project+BUILD_OUTPUT_SUFIX, 'win')


def get_pac_dir(code_root, project):
    return os.path.join(code_root, BUILD_OUTPUT, project+BUILD_OUTPUT_SUFIX, IMG)


def get_idh_root(code_root):
    return os.path.abspath(os.path.join(code_root, os.path.pardir, IDH_ROOT))


def get_idh_code_dir(code_root):
    return os.path.abspath(os.path.join(get_idh_root(code_root), IDH_CODE))


def get_idh_verify_dir(code_root):
    return os.path.abspath(os.path.join(code_root, os.path.pardir, IDH_VERIFY))

def build(project, modules=[], simulator=False, mode = '', log_file = ''):
    #check dir
    if(not os.path.exists(BUILD_OUTPUT)):
        os.mkdir(BUILD_OUTPUT)
    mocor_dir = os.path.abspath(os.getcwd())
    val = 0
    if(len(modules) != 0):
        mod_str = ' '
        mod_str = mod_str.join(modules)
    #prepare cmd line
        if(mode == 'clean'):
            cmd = f"make p={project} m=\"{mod_str}\" clean -j32 >> {log_file}"
            val = exec_cmd(cmd)
            if(val != 0):
                return(val)
        cmd = f"make p={project} m=\"{mod_str}\" -j32 >> {log_file}"
        val = exec_cmd(cmd)
        if(val != 0):
            return(val)
        cmd = f"make p={project} image -j32 >> {log_file}"
        val = exec_cmd(cmd)

    elif simulator:
        cmd = f"make p={project} m=simulator_main clean >> {log_file}"
        val = exec_cmd(cmd)
        if(val != 0):
            return(val)
        cmd = f"make p={project} m=simulator_main VS2010=1 -j32 >> {log_file}"  #w217 use 2010.
        val = exec_cmd(cmd)
        if(val != 0):
            return(val)
        #build in vs2010
        sln = os.path.join(get_simulator_dir(mocor_dir, project), SIMULATOR)
        if(DEBUG):
            print(sln)
        cmd = f"\"{VS2010EXE}\" /build Debug  {sln} msdevkernel /Out ../log.txt"
        val = exec_cmd(cmd)
        #copy log
        with open('../log.txt', 'r') as fin:
            with open(log_file, 'a') as fout:
                log_lines = fin.readlines()
                fout.writelines(log_lines)
        if(val != 0):
            return(val)
        #verify
        msdevkernel = os.path.join(get_simulator_dir(mocor_dir, project), 'bin', 'msdevkernel.dll')
        if(not os.path.exists(msdevkernel)):
            print('Error: Verify Simulator Build fail.')
            return 1
    else:
        if mode == 'new':
            cmd = f"make p={project} new verify_envir=1 -j32 >> {log_file}"
            val = exec_cmd(cmd)
            #val = exec_cmd(cmd)
            if(val == 0):
                print('Check pac after full build new')
                pac_file = os.path.join(get_pac_dir(mocor_dir, project), project+'.pac')
                if(not os.path.exists(pac_file)):
                    return 11
        else:
            cmd = f"make p={project} -j32 >> {log_file}"
            val = exec_cmd(cmd)
            if(val == 0):
                print('Check pac after full build')
                pac_file = os.path.join(get_pac_dir(mocor_dir, project), project+'.pac')
                if(not os.path.exists(pac_file)):
                    return 11
    return val



def idh(branch, project, log_file = ''):
    mocor_dir = os.path.abspath(os.getcwd())
    idh_root = get_idh_root(mocor_dir)
    idh_code = get_idh_code_dir(mocor_dir)
    val = prepare_folder(idh_root)
    val = prepare_folder(idh_code)
    script_path = os.path.split(os.path.realpath(__file__))[0]
    idh_script = os.path.join( mocor_dir, 'project', 'build_config', 'idh.pl')

    if(DEBUG):
        print(f'mocor_dir: {mocor_dir}')
        print(f'idh_script: {idh_script}')

    board_list = []
    if(project == None):
        with open(os.path.join(script_path, 'boards.json')) as jf:
            branches = json.load(jf)
        for b in branches:
            if (branch == b):
                board_list = [item['name'] for item in branches[b]]
    else:
        board_list.append(project)
    if(DEBUG):
        print(board_list)
    if(len(board_list)==0):
        print('Error: No boards found.')
        return 1
    #check build dir
    for board in board_list :
        if(not os.path.exists(get_build_dir(mocor_dir, board))):
            print(f'Error: build output not exist {get_build_dir(mocor_dir, board)}')
            return 11

    cmd = f"{PERLEXE} {idh_script} {mocor_dir} {idh_code} > {log_file} 2>&1"
    val = 0
    val = exec_cmd(cmd)
    if(val != 0):
        return(val)
    for board in board_list :
        cmd = f"{PERLEXE} {idh_script} {mocor_dir} {os.path.join(idh_root, board)} {board} > ./log.txt 2>&1"
        val = exec_cmd(cmd)
        #copy log
        with open('./log.txt', 'r') as fin:
            with open(log_file, 'a') as fout:
                log_lines = fin.readlines()
                fout.writelines(log_lines)
        if(val != 0):
            break

    return val


