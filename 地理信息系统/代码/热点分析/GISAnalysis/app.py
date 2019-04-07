from flask import Flask, render_template
from analysis import *
from data_manager import get_points

app = Flask(__name__)


# 热力图路由
@app.route('/analysis/<time_interval>')
def analysis(time_interval):
    points = get_points(time_interval)
    hot_list = analysis_hot_power(points)
    return render_template('analysis.html', hot_list=json.dumps(hot_list), time_interval=time_interval)


# 热点区域路由
@app.route('/hot/<time_interval>')
def hot(time_interval):
    points = get_points(time_interval)
    centers = mean_shift(points)
    centers_info = json.dumps(get_address(centers), ensure_ascii=False)
    return render_template('hot.html', time_interval=time_interval,
                           centers=json.dumps(centers), centers_info=centers_info)


# 程序入口
if __name__ == '__main__':
    app.run()
