from OpenGL.GL import *
import glfw
import numpy as np
import pandas as pd


# 生成贝塞尔曲线上的点
def Bezier(points):
    data = np.array([
        points[0],
        points[1],
        points[2],
        points[3]
    ]).T
    points_ = []
    for index in range(100):
        t = (index + 1) / 100
        new_x = (1 - t) ** 3 * data[0, 0] + 3 * t * (1 - t) ** 2 * data[0, 1] + 3 * t ** 2 * (1 - t) * data[
            0, 2] + t ** 3 * data[0, 3]
        new_y = (1 - t) ** 3 * data[1, 0] + 3 * t * (1 - t) ** 2 * data[1, 1] + 3 * t ** 2 * (1 - t) * data[
            1, 2] + t ** 3 * data[1, 3]
        new_z = (1 - t) ** 3 * data[2, 0] + 3 * t * (1 - t) ** 2 * data[2, 1] + 3 * t ** 2 * (1 - t) * data[
            2, 2] + t ** 3 * data[2, 3]
        points_.append([new_x, new_y, new_z])
    return points_


# is_bezier 为真时表示贝塞尔曲线转B样条曲线
# 反之则为B样条曲线转贝塞尔曲线
def transform(is_bezier=True):
    global points
    if len(points) >= 4:
        points = points[:4]

        np_bezier = np.mat([
            [-1, 3, -3, 1],
            [3, -6, 3, 0],
            [-3, 3, 0, 0],
            [1, 0, 0, 0]
        ])

        np_spline = np.mat([
            [-1, 3, -3, 1],
            [3, -6, 3, 0],
            [-3, 0, 3, 0],
            [1, 4, 1, 0]
        ]) * 1 / 6

        if is_bezier:
            points = np.array(np_spline.I*np_bezier*points).reshape((4, 2))
        else:
            points = np.array(np_bezier.I*np_spline*points).reshape((4, 2))


# 扩展曲线中的点
# 输入一组点
# 返回扩展后的点
def extend_lines(points):
    points_ = []
    if len(points) % 3 == 1:
        for i in range(int(len(points) / 3)):
            points_.append(Bezier(points[i * 3:i * 3 + 4]))
    new_points = []
    for i in range(len(points_)):
        for j in range(100):
            new_points.append(points_[i][j])
    return new_points


# 将曲线中的点绕纵轴旋转 360次 * 1度 后的点集
def rotate_lines(points):
    new_points = []
    for index in range(360):
        t_points = []
        for j in range(len(points)):
            tp = np.mat(np.array([points[j][0], points[j][2]])).T
            tp = np.mat([[np.cos(index / 180 * np.pi), -np.sin(index / 180 * np.pi)],
                         [np.sin(index / 180 * np.pi), np.cos(index / 180 * np.pi)]]) * tp
            tp = np.array(tp).T
            t_points.append([tp[0][0], points[j][1], tp[0][1]])
        new_points.append(t_points)
    return new_points


# 将曲线中的点按照xOz平面上的圆形轨迹平移
def move_lines(points, r=0.05):
    new_points = []
    move = np.mat([r, 0]).T
    for index in range(360):
        t_points = []
        for j in range(len(points)):
            tp = np.mat([[np.cos(index / 180 * np.pi), -np.sin(index / 180 * np.pi)],
                         [np.sin(index / 180 * np.pi), np.cos(index / 180 * np.pi)]]) * move
            tp = np.array(tp)
            t_points.append([points[j][0] + tp[0][0], points[j][1], points[j][2] + tp[1][0]])
        new_points.append(t_points)
    return new_points


# 每4个相邻的可以构成矩形的点划分为2个三角形
def triangle_points(points):
    triangles = []
    hang = len(points)
    lie = len(points[0])
    for index in range(hang - 1):
        for j in range(lie - 1):
            triangles.append([index * lie + j, (index + 1) * lie + j, index * lie + j + 1])
            triangles.append([(index + 1) * lie + j, index * lie + j + 1, (index + 1) * lie + j + 1])
    index = hang - 1
    for j in range(lie - 1):
        triangles.append([index * lie + j, j, index * lie + j + 1])
        triangles.append([j, index * lie + j + 1, j + 1])
    return np.array(triangles) + 1


def draw_teapot(file_name='teapot_high.obj'):
    # 曲线控制点
    # 茶壶本体
    print('茶壶模型' + file_name + '生成中')
    points = [
        [0, -2, 0],
        [-0.25, -2, 0],
        [-0.5, -2, 0],
        [-0.75, -2, 0],
        [-1, -2, 0],
        [-2, -1, 0],
        [-0.75, 1, 0],
        [0, 1, 0],
        [0, 1.2, 0],
        [-0.25, 1.5, 0],
        [0, 1.5, 0],
        [0, 1.5, 0],
        [0, 1.5, 0]
    ]
    points = np.array(points) / 5
    points = np.array(extend_lines(points))
    point_matrix = rotate_lines(points)
    triangles = triangle_points(point_matrix)

    # 茶壶把
    bar = [
        [0, 2, 0],
        [3, 4, 0],
        [4, 0, 0],
        [0, 0, 0]
    ]
    bar = np.array(bar) / 5
    bar[:, 0] -= 0.16
    bar[:, 1] -= 0.35
    bar_points = np.array(extend_lines(bar))
    bar_points = move_lines(bar_points)
    bar_triangles = triangle_points(bar_points)

    # 茶壶嘴
    mouth = [
        [-0.25, -1, 0],
        [-0.75, -1, 0],
        [-1.25, 1, 0],
        [-1.75, 1, 0]
    ]
    mouth = np.array(mouth) / 5
    mouth_points = np.array(extend_lines(mouth))
    mouth_points = move_lines(mouth_points)
    mouth_triangles = triangle_points(mouth_points)

    with open(file_name, 'w') as f:
        for i in range(len(point_matrix)):
            for j in range(len(point_matrix[i])):
                f.write('v ' + str(point_matrix[i][j][0]) + ' ' + str(point_matrix[i][j][1]) + ' ' + str(
                    point_matrix[i][j][2]) + '\n\r')
        print('茶壶模型' + file_name + '生成 16.7%')
        for i in range(len(bar_points)):
            for j in range(len(bar_points[i])):
                f.write('v ' + str(bar_points[i][j][0]) + ' ' + str(bar_points[i][j][1]) + ' ' + str(
                    bar_points[i][j][2]) + '\n\r')
        print('茶壶模型' + file_name + '生成 33.3%')
        for i in range(len(mouth_points)):
            for j in range(len(mouth_points[i])):
                f.write('v ' + str(mouth_points[i][j][0]) + ' ' + str(mouth_points[i][j][1]) + ' ' + str(
                    mouth_points[i][j][2]) + '\n\r')
        print('茶壶模型' + file_name + '生成 50%')
        for i in range(len(triangles)):
            f.write('f ' + str(triangles[i][0]) + ' ' + str(triangles[i][1]) + ' ' + str(triangles[i][2]) + '\n\r')
        print('茶壶模型' + file_name + '生成 66.7%')
        for i in range(len(bar_triangles)):
            f.write('f ' + str(bar_triangles[i][0] + 144000) + ' ' + str(bar_triangles[i][1] + 144000) + ' ' + str(
                bar_triangles[i][2] + 144000) + '\n\r')
        print('茶壶模型' + file_name + '生成 83.3%')
        for i in range(len(mouth_triangles)):
            f.write('f ' + str(mouth_triangles[i][0] + 180000) + ' ' + str(mouth_triangles[i][1] + 180000) + ' ' + str(
                mouth_triangles[i][2] + 180000) + '\n\r')

    print('茶壶模型' + file_name + '生成完成')


# 绘制闭包
def Bibao(points):
    glColor3f(0, 0, 1)
    glBegin(GL_LINE_STRIP)
    glVertex2f(points[0][0], points[0][1])
    glVertex2f(points[1][0], points[1][1])
    glVertex2f(points[2][0], points[2][1])
    glVertex2f(points[3][0], points[3][1])
    glEnd()
    glColor3f(1, 0, 0)
    glBegin(GL_POINTS)
    glVertex2f(points[0][0], points[0][1])
    glVertex2f(points[1][0], points[1][1])
    glVertex2f(points[2][0], points[2][1])
    glVertex2f(points[3][0], points[3][1])
    glEnd()


# 绘制贝塞尔曲线
# 输入为包含四个点的列表
# 每个点包含横纵坐标
def drawBezier(points):

    Bibao(points=points)

    # 将 4 个点的横纵坐标转化为 2 × 4 矩阵
    data = np.array([
        points[0],
        points[1],
        points[2],
        points[3]
    ]).T

    glColor3f(0, 1, 0)

    # 绘制连续的折线
    glBegin(GL_LINE_STRIP)
    for index in range(100):

        # t 值从 0.01 每次递增 0.01
        t = (index + 1) / 100

        # 贝塞尔曲线上点的坐标
        new_x = (1-t)**3*data[0, 0] + 3*t*(1-t)**2*data[0, 1] + 3*t**2*(1-t)*data[0, 2] + t**3*data[0, 3]
        new_y = (1-t)**3*data[1, 0] + 3*t*(1-t)**2*data[1, 1] + 3*t**2*(1-t)*data[1, 2] + t**3*data[1, 3]

        # 绘制折线
        glVertex2f(new_x, new_y)
    glEnd()


# 绘制B样条曲线
# 输入为包含四个点的列表
# 每个点包含横纵坐标
def drawBsplines(points):

    Bibao(points=points)

    # 将 4 个点的横纵坐标转化为 2 × 4 矩阵
    data = np.array([
        points[0],
        points[1],
        points[2],
        points[3]
    ]).T

    glColor3f(0, 1, 0)

    # 绘制连续的折线
    glBegin(GL_LINE_STRIP)
    for index in range(100):
        # t 值从 0.01 每次递增 0.01
        t = (index + 1) / 100

        # B样条曲线上点的坐标
        new_x = 1 / 6 * (1 - t) ** 3 * data[0, 0] + 1 / 6 * (3 * t ** 3 - 6 * t ** 2 + 4) * data[0, 1] + 1 / 6 * (-3 * t ** 3 + 3 * t ** 2 + 3 * t + 1) * data[
            0, 2] + 1 / 6 * t ** 3 * data[0, 3]
        new_y = 1 / 6 * (1 - t) ** 3 * data[1, 0] + 1 / 6 * (3 * t ** 3 - 6 * t ** 2 + 4) * data[1, 1] + 1 / 6 * (-3 * t ** 3 + 3 * t ** 2 + 3 * t + 1) * data[
            1, 2] + 1 / 6 * t ** 3 * data[1, 3]

        # 绘制折线
        glVertex2f(new_x, new_y)
    glEnd()


# 绘制多控制点的曲线
def draw_lines(points, bezier=True):
    if len(points) >= 4:
        if bezier:
            if len(points) % 3 == 1:
                for i in range(int(len(points)/3)):
                    drawBezier(points[i*3:i*3+4])
        else:
            for i in range(len(points)-3):
                drawBsplines(points[i:i+4])


# 选中控制点
def select_point(x, y):
    x -= 400
    x /= 8
    y -= 300
    y *= -1
    y /= 6
    for c in range(len(points)):
        if abs(points[c][0]-x/50) < 0.02 and abs(points[c][1]-y/50) < 0.02:
            return c
    return None


# 海伦公式
def helen(x1, x2, x3, y1, y2, y3):
    c1 = ((x1-x2)**2 + (y1-y2)**2) ** 0.5
    c2 = ((x1-x3)**2 + (y1-y3)**2) ** 0.5
    c3 = ((x3-x2)**2 + (y3-y2)**2) ** 0.5
    p = (c1+c2+c3) / 2
    return (p * (p - c1) * (p - c2) * (p - c3)) ** 0.5


# 选择最近的点
def shortest_point(x, y):
    global points
    distance = []
    key_num = []
    x -= 400
    x /= 400
    y -= 300
    y *= -1
    y /= 300
    for i in range(len(points)-1):
        s = helen(x, points[i][0], points[i+1][0], y, points[i][1], points[i+1][1])
        distance.append(s/((points[i][0]-points[i+1][0])**2+(points[i][1]-points[i+1][1])**2)**0.5)
        key_num.append(i)
    df = pd.DataFrame()
    df.insert(0, 'distance', distance)
    df.insert(1, 'key_num', key_num)
    df = df.sort_values(by='distance')
    key_num = list(df['key_num'])
    return key_num[0]


# 键盘事件
mark = True
mark_r = True
mark_b = True
def key_callback(window, key, scancode, action, mods):
    global mark
    global mark_r
    global mark_b
    if mark:
        mark = False
        if key == 82:
           if mark_r:
               mark_r = False
           else:
               mark_r = True
        elif key == 83:
            with open('points.txt', 'w') as f:
               for obj in points:
                   f.write('v '+str(obj[0])+' '+str(obj[1])+'\r\n')
            print('点集 points.txt 保存成功')
        elif key == 86:
           draw_teapot()
        elif key == 70:
            if mark_b:
                transform(mark_b)
                mark_b = False
            else:
                transform(mark_b)
                mark_b = True
            if mark_r:
                mark_r = False
            else:
                mark_r = True
    else:
        mark = True


# 添加控制点
def add_point():
    global points
    if len(points) >= 2:
        x, y = glfw.get_cursor_pos(window)
        pos = shortest_point(x, y)
        new_points = list(points[:pos+1])
        new_points.append([(points[pos][0]+points[pos+1][0])/2, (points[pos][1]+points[pos+1][1])/2])
        new_points += list(points[pos+1:])
        points = np.array(new_points)


# 删除控制点
def delete_point():
    global points
    if len(points) >= 1:
        x, y = glfw.get_cursor_pos(window)
        num = select_point(x, y)
        if not num == None:
            points = np.delete(points, num, axis=0)


# 鼠标事件
# 鼠标移动事件
mark_move = False
def cursor_position_callback(window, x, y):
    if mark_move:
        num = select_point(x, y)
        if not num == None:
            x -= 400
            x /= 8
            y -= 300
            y *= -1
            y /= 6
            points[num] = [x/50, y/50]


# 鼠标点击事件
def mouse_button_callback(window, button, action, mods):
    global mark_move
    if action == 1:
        if button == 0:
            mark_move = True
        if button == 2:
            add_point()
        if button == 1:
            delete_point()
    elif action == 0:
        mark_move = False


glfw.init()
window = glfw.create_window(800, 600, 'kiko', None, None)
glfw.make_context_current(window)

glfw.set_key_callback(window, key_callback)
glfw.set_mouse_button_callback(window, mouse_button_callback)
glfw.set_cursor_pos_callback(window, cursor_position_callback)

# 曲线控制点
points = [
    [0, -2],
    [-0.25, -2],
    [-0.5, -2],
    [-0.75, -2],
    [-1, -2],
    [-2, -1],
    [-0.75, 1],
    [0, 1],
    [0, 1.2],
    [-0.25, 1.5],
    [0, 1.5],
    [0, 1.5],
    [0, 1.5]
]
points = np.array(points) / 5

glPointSize(10)

print('''
按鼠标左键选中并移动控制点
按鼠标中键在边上添加控制点
按鼠标右键选中并删除控制点
(控制点数量不满足约束条件时将不显示)
按 s 键保存点的位置信息
按 r 键切换贝塞尔曲线和B样条曲线
按 v 键生成并保存茶壶模型
按 f 键切换贝塞尔曲线控制点和B样条曲线控制点
(只保留最前面的 4 个控制点)
(请确保您已关闭了输入法)
''')

while not glfw.window_should_close(window):
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    glClearColor(0, 0, 0, 0)
    if mark_r:
        draw_lines(points)
    else:
        draw_lines(points, bezier=False)
    glfw.swap_buffers(window)
    glfw.poll_events()

# 释放 GLFW 的内存
glfw.terminate()
