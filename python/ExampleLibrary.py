#encoding=utf-8

import string
import random
from robot.api import logger
import logging
import sys
import re

class ExampleLibrary:

    ROBOT_LIBRARY_SCOPE = 'GLOBAL'
    ROBOT_LIBRARY_VERSION = 0.1
    ROBOT_LIBRARY_DOC_FORMAT = 'reST'

    def __init__(selfself, arg = 1):
        """Library 文档 *斜体*"""
        print('Library arg %s ' % arg)
        pass

    def gen_nums(self, counts,log, *paraName1):
        """生成随机数字符串"""
        s  = self._gen_nums(counts)
        print('*INFO* Get random number string=%s' % s)
        print("get para",log)
        strMatch = r'(?<=' + paraName1[0] + r')\s*\d+\.?\d*'
        print(strMatch)
        pattern = re.compile(strMatch)
        #pattern = re.compile(r'(?<=result: =)\d+\.?\d*')
        result = pattern.findall(log)
        result[0] = result[0].lstrip()
        print("result ====",result)
        return result

    def _gen_nums(self, counts):

        li = string.digits
        s = ''
        for n in range(0, int(counts)):
            s += li[random.randint(0 , len(li) - 1)]
        return s

    def pluse_data_get(self, log, *paraName1):

        resultData = list()
        logTag = 0
        logList = log.split('\n')
        #print(logList)
        for i in range(0, len(logList)):
            #print(logList[i])
            strs = 'num, 0'
            find_result = logList[i].find(strs)
            if find_result != -1:
                print('脉冲分析！！！')
                logTag = i
                break
        for i in range(0, 512):
            #print(logList[i + logTag])
            strMatch = r'(?<=' + paraName1[0] + r')\s*\d+\.?\d*'
           # print(strMatch)
            pattern = re.compile(strMatch)
            result = pattern.findall(logList[i + logTag])
            num = int(str(result[0]).strip())
            resultData.append(str(num))
        print('脉冲分析结果：')
        print(resultData)

        return str(0)

    def energy_data_get(self, log, *paraName1):
        resultData = list()
        logTag = 0
        logList = log.split('\n')
        #print(logList)
        for i in range(0, len(logList)):
            #print(logList[i])
            strs = 'max speed:'
            find_result = logList[i].find(strs)
            if find_result != -1:
                print('能谱分析！！！')
                logTag = i
                break
        for i in range(0, 8192):
            #print(logList[i + logTag])
            strMatch = r'(?<=' + paraName1[0] + r')\s*\d+\.?\d*'
           # print(strMatch)
            pattern = re.compile(strMatch)
            result = pattern.findall(logList[i + logTag + 1])
            num = int(str(result[0]).strip())
            resultData.append(str(num))
        print('能谱分析结果：')
        print(resultData)
        return str(0)

    def win_data_get(self, log, *paraName1):
        resultData = list()
        logTag = 0
        logList = log.split('\n')
        print(logList)
        for i in range(0, len(logList)):
            #print(logList[i])
            strs = 'max speed:'
            find_result = logList[i].find(strs)
            if find_result != -1:
                logTag = i
                break
        for i in range(0, 5):
            #print(logList[i + logTag])
            strMatch = r'(?<=' + paraName1[0] + r')\s*\d+\.?\d*'
           # print(strMatch)
            pattern = re.compile(strMatch)
            result = pattern.findall(logList[i + logTag + 1])
            num = int(str(result[0]).strip())
            resultData.append(str(num))
        print('窗口结果：')
        print(resultData)
        return str(0)

    def read_pressure_sensor(self, sensorData, *paraDb):

        strMatch = r'(?<=' + 'iic read pressure: ' + r')\s*\d+\.?\d*'
        pattern = re.compile(strMatch)
        result = pattern.findall(sensorData)
        data = int(str(result[0]).strip())
        print(data)
        data = paraDb[0][1] * (data * 6.144 / 32767.0) + paraDb[0][2]
        print("read_pressure_sensor = ", data , "kPa")

    def read_delta_pressure_sensor(self, sensorData, *paraDb):

        strMatch = r'(?<=' + 'iic read delta pressure: ' + r')\s*\d+\.?\d*'
        pattern = re.compile(strMatch)
        result = pattern.findall(sensorData)
        data = int(str(result[0]).strip())
        print(data)
        data = paraDb[0][3] * (data * 6.144 / 32767.0) + paraDb[0][5]
        print("read_delta_pressure_sensor = ", data , "Pa")

    def read_pressure_temperature_sensor(self, sensorData, *paraDb):
        R1 = 10000
        R2 = 1000
        V0 = 5
        strMatch = r'(?<=' + 'iic read pressure temperature: ' + r')\s*\d+\.?\d*'
        pattern = re.compile(strMatch)
        result = pattern.findall(sensorData)
        data = int(str(result[0]).strip())
        print(data)
        data = (data * 0.256 / 32767.0)
        data = (0.258137 * (((data * (R1 * (R1 + R2))) + (V0 * R1 * R2)) / ((V0 - data) * (R1 + R2) - (V0 * R2)))) - 257.990783;
        print("read_delta_pressure_sensor = ", data, "C")

    def arg_demo(self, arg1, arg2 = 2, *args):
        print(arg1, arg2)
        for arg in  args:
            sys.__stdout__.write('Got arg %s\n' % arg)

    def freearg_deom(self, **freearg):
        for name, value in  freearg.items():
            print(name, value)

class OtherLibrary:

    def __init__(self):
        self._counter = 0
        pass

    def count(self):
        """用于计数器"""
        self._counter += 1
        logger.debug('self._counter += 1')
        return self._counter

    def clear_counter(self):
        """clear counter"""
        self._counter = 0
        logger.info('self._counter = 0')
