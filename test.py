import sys,requests
def output(str,file):
    with open(file, "w") as f:
        sys.stdout=f
        print(str)
def input(file):   
    with open(file,'r') as f:
        line=f.readline()
        n1,n2=map(int,line.split())
        return n1,n2
def getkey():
    with open(".api_key",'r') as f:
        line=f.readline()
        return line
def judge(key):
    url = "https://api.deepseek.com/user/balance"
    headers = {
        'Accept': 'application/json',
        'Authorization': f'Bearer {key}'
    }
    try:
        response = requests.request("GET", url, headers=headers, data={})
        if response.status_code == 200:
            return 0  # 密钥有效
        elif response.status_code == 401:
            return 1  # 密钥无效或未授权
        elif response.status_code >= 500:
            return 3  # 服务器错误
        else:
            return 4  # 其他HTTP错误
    except requests.exceptions.ConnectionError:
        return 2  # 网络连接错误
    except requests.exceptions.Timeout:
        return 2  # 网络超时
    except Exception:
        return 4
    

if(judge(getkey())!=0):
    print('sb')
else:
    print('ok')