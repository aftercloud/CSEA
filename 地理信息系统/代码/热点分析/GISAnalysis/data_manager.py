import pymssql as pl


# 获取轨迹数据
def get_points(time_interval):
    conn = pl.connect(server='localhost', user='sa', password='123', database='GIS')
    cur = conn.cursor()
    if time_interval == 'peak':
        cur.execute("select lng, lat from dbo.taxi where (time between cast('2014-05-07 07:00:00' as datetime) and cast('2014-05-07 09:00:00' as datetime)) or (time between cast('2014-05-07 17:00:00' as datetime) and cast('2014-05-07 19:00:00' as datetime));")
    elif time_interval == 'normal':
        cur.execute("select lng, lat from dbo.taxi where time between cast('2014-05-07 9:00:00' as datetime) and cast('2014-05-07 17:00:00' as datetime);")
    else:
        cur.execute("select lng, lat from dbo.taxi where (time between cast('2014-05-07 00:00:00' as datetime) and cast('2014-05-07 04:00:00' as datetime)) or (time between cast('2014-05-07 22:00:00' as datetime) and cast('2014-05-07 23:59:59' as datetime));")
    points = cur.fetchall()
    cur.close()
    conn.close()
    return points
