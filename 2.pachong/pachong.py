#coding=utf-8

# http://zhsk.12348.gov.cn/qa.wap/ 采集所有内容，通过谷歌浏览器F12，分析Sources标签和Newwork标签，得出如下结论：
# 要点1：网页http://zhsk.12348.gov.cn/qa.wap/query/lb?cls=2，最后一个数字范围是2~17，对应了各大模块。
# 
# 要点2：通过下面的ajax请求，可以返回当前模块一共有多少个条目，response就是一个数字：
#         url: '/qa.wap/query/GetCount' + "?rid=" + encodeURIComponent('1/6') + "&t=" + Math.random(),
#         type: "Get",
#         【说明】：encodeURIComponent中的6代表了第几个模块，通过该函数后变为了1%F6，可以在python中引用urllib包中parse模块的quote和unquote
# 
# 要点3：模块数据分页查询。加载页面之前会调用ajax请求，点击下一页会重发该请求更新页面数据，num会变化：
#           url: "/qa.wap/query/GetLb",
#           type: "post",
#           data: { rid: '1/17', pagesize: pagesize, pageindex: num },  #这里rid和上面的对应，pagesize表示一个页面几个条目，固定是30，pageindex为第几个页面数据。
#           dataType: "json",
#       response数据为，这里的id是关键：
#           {"total":46,
#               "list":[
#                   {"type":"刑事诉讼法","typeid":"17","q":"司法机关应当如何处置依法不负刑事责任的精神病人？","id":"CA00000197"},
#                   {"type":"刑事诉讼法","typeid":"17","q":"检察机关收缴犯罪赃款赃物的条件和程序是什么？","id":"CA00000204"}。。。
#要点4：通过上一步中的id，可以通过构建url来查询出结果数据，url例如为：http://zhsk.12348.gov.cn/qa.wap/query/detail?id=CA00000197，剩下的就是解析该页面的数据了。
#
# 要点5：最终html页面数据解析。查看网页发现第一个clearfix标签下又分了clearfix和clearfix mtp两个，分别对应问答字段，这里可以先找到第0个标签，然后在其中搜索
#后面的两个，找p标签。不然可能会找重复。
# 
# 最终将结果存放到json文件中：
#   [{"title" : "民法总则",
#        "list" : [
#            {"question":"民法的调整范围是什么？","ask":"xxxx","answer":"xxxxx"},
#            {"question":"民法的调整范围是什么？","ask":"xxxx","answer":"xxxxx"}
#        ]
#   }]

import requests
from bs4 import BeautifulSoup
import urllib.parse
import random
import re
import json 

#测试
headers = {'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/70.0.3538.110 Safari/537.36'}
r = requests.get('http://zhsk.12348.gov.cn/qa.wap/', headers=headers)
print(r.status_code)
#print(r.text) #返回页面代码

#测试2
#获取模块总条目--ajax请求
url = '/qa.wap/query/GetCount' + "?rid=" + urllib.parse.quote('1/6') + "&t=" + str(random.random())
base_url = 'http://zhsk.12348.gov.cn'+url
headers = {
    'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/70.0.3538.110 Safari/537.36',
    'X-Requested-With': 'XMLHttpRequest'
}
r = requests.get(base_url, headers=headers)
print(r.status_code)
print(r.json()) 
#获取一个页面的数据 ajax请求
url = '/qa.wap/query/GetLb'
base_url = 'http://zhsk.12348.gov.cn'+url
data = { 'rid': '1/6', 'pagesize': 30, 'pageindex': 1 }
r = requests.post(base_url, data=data, headers=headers)
print(r.status_code)
val = r.json()
print(type(val)) #dict数据
#获取id
id = val['list'][0]['id']
print(id)
#获取一个问题的答案 ajax请求
base_url = 'http://zhsk.12348.gov.cn/qa.wap/query/detail?id='+id
r = requests.get(base_url, headers=headers)
print(r.status_code)
#print(r.text) 
#解析答案
soup = BeautifulSoup(r.text, 'lxml')
q = soup.find_all('h4')
print(q[0].string) #问题标题
p = soup.find_all(attrs={'class': 'clearfix mtb'})
print(p[0].find_all('p')[0].string) #问--该项可能没有
p = soup.find_all(attrs={'class': 'clearfix'})
print(p[0].find_all('p')) #答

######开始######
url = 'http://zhsk.12348.gov.cn'
headers = {
    'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/70.0.3538.110 Safari/537.36',
    'X-Requested-With': 'XMLHttpRequest'
}        
module_name = ["民法总则","物权法","合同法","侵权责任法","劳动合同法","消费者权益保护","居民身份证法","道路交通安全法",
"治安管理处罚法","妇女权益保障法","未成年人保护法","人民调解法","行政许可法","食品安全法","婚姻法和继承法","刑事诉讼法"]
write_list = [] #最后构建json

# 答案p标签的内容拼接
def strcat(tag):
    if len(tag)>0: #有p
        strs = ''
        for i in range(len(tag)):
            if len(tag[i].contents)>0: #有的p没有东西
                strs = strs + tag[i].string
        return strs
    else:
        assert False
    return ''

for num in range(17, 18): #后面那个范围要减1
    pagesize = 30 #一个页面条目数目
    module_dict = {}
    module_dict['title'] = module_name[num-2]

    #获取模块数量
    if num==16: #婚姻法和继承法特殊
        rid = '1/41;1/50'
    else:
        rid = '1/' + str(num)
    base_url = url + '/qa.wap/query/GetCount' + "?rid=" + urllib.parse.quote(rid) + "&t=" + str(random.random())
    r = requests.get(base_url, headers=headers)
    #print(r.status_code)
    module_item_num = int(r.json()) #该模块总数目
    
    if module_item_num%pagesize==0 :
        page_num = int(module_item_num / pagesize) #页数目
    else:
        page_num = int(module_item_num / pagesize) + 1
    print(module_name[num-2]+' 模块总条目数：'+str(module_item_num)+'总页数：'+str(page_num)+',开始抓取...')
    
    #获取页面数据
    list_value = []
    for page in range(page_num):
        print(module_name[num-2]+' 模块开始抓取第'+str(page+1)+'页的数据...')
        base_url = url + '/qa.wap/query/GetLb'
        data = { 'rid': rid, 'pagesize': pagesize, 'pageindex': page+1 }
        r = requests.post(base_url, data=data, headers=headers)
        #print(r.status_code)
        val = r.json()
        #根据id，解析对应的页面数据
        for item in val['list']:
            value = {}
            value['ask'] = ''
            value['answer'] = ''

            id = item['id']
            base_url = 'http://zhsk.12348.gov.cn/qa.wap/query/detail?id='+id
            r = requests.get(base_url, headers=headers)
            #print(r.status_code)
            #解析答案
            soup = BeautifulSoup(r.text, 'lxml')

            #问题标题
            q = soup.find_all('h4')
            assert len(q)==1
            value['question'] = q[0].string

            #问--该项可能没有
            p = soup.find_all(attrs={'class': 'clearfix mtb'})
            assert len(p) <= 1
            if len(p) == 1:
                value['ask'] = p[0].find_all('p')[0].string          

            #答 --可能有多个，这里clearfix 和clearfix mtb都匹配出来了，且完全匹配clearfix的有两个，问答的内容都在第0个clearfix里面，需再次查找缩小范围
            #第一次find_all之后：
            #<dev class="clearfix">
            #   <dev class="clearfix mtb">
            #   <dev class="clearfix">
            p = soup.find_all(attrs={'class': 'clearfix'}) 
            p = p[0].find_all(attrs={'class': 'clearfix'}) #在范围内再次查找，此时还是会匹配出两个
            for j in range(len(p)):
                if len(p[j].attrs['class'])==1: #attrs内容是['class':['clearfix']]或['class':['clearfix','mtb']]，该判断只留下前面一种
                    ps = p[j].find_all('p') 
                    value['answer'] = strcat(ps)

            #【劳动合同法特有的class=dagraybox】
            p = soup.find_all(attrs={'class': 'dagraybox'})
            if len(p)>0:
                ps = p[0].find_all('p') 
                value['answer'] = strcat(ps)
            list_value.append(value)
    module_dict['list'] = list_value
    write_list.append(module_dict)

#保存成json文件
    json_file_text = json.dumps(write_list, ensure_ascii=False)
    file = open("file_pachong.json", "w", encoding='utf-8')
    file.write(json_file_text)
    file.close()