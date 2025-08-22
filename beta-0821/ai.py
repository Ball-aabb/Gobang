from openai import OpenAI
import time,requests
def output(content,filename):
    with open(filename,"w") as f:
        f.write(str(content))
def input(file):   
    with open(file,'r') as f:
        line=f.readline()
        n1,n2=map(int,line.split())
        return n1,n2
def getkey():
    with open(".apikey",'r') as f:
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
        return 4  # 其他错误

def judge2():
    temp=judge(getkey())
    output(temp,".key_state")
    if(temp!=0):
        exit(0)
    else:
        print("key ok!")
def ask(question):
    messages.append({"role": "user", "content": question})
    response = client.chat.completions.create(model="deepseek-chat",messages=messages)
    assistant_reply = response.choices[0].message.content
    messages.append({"role": "assistant", "content": assistant_reply})
    return assistant_reply

if __name__ == "__main__":
    output('0','.out_state')
    output(' ','.out')
    output('0 0','.in_state')
    output('1 1','.in')
    output('-1','.apikey')
    output('-1 -1','.key_state')
    k1,k2=input(".key_state")
    while k1==-1 and k2==-1:
        time.sleep(1)
        k1,k2=input(".key_state")
    judge2()
    client = OpenAI(api_key=getkey(),base_url="https://api.deepseek.com")
    messages = [{"role": "system", "content": "你是棋艺一般的五子棋棋手棋盘15×15我以数字a b告知落子行列请用相同格式回复。"}]
    tx=ty=0
    while 1:
        in1,in2=input(".in_state")
        while in1==0 and in2==0:
            time.sleep(1)
            in1,in2=input(".in_state")
        in1,in2=input(".in")
        output('0 0','.in_state')
        tx=tx+1
        ty=ty+1
        strx=str(tx)
        stry=str(ty)
        output(1,'.out_state')
        output(strx+' '+stry,'.out')
        #a1 = ask("15 15")
        #print(a1) 