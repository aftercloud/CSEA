import requests
from flask import request
from flask_api import FlaskAPI

app = FlaskAPI(__name__)


@app.route('/', methods=['GET', 'POST'])
def calc():
    if request.method == 'GET':
        return {}
    r = requests.post("http://localhost:8080/", data={"key": str(request.data)})
    return {'fee': str(r.text)}


if __name__ == "__main__":
    app.run()

# {"Space": 2, "Vip": 0, "Begin": "迪拜", "End": "乌鲁木齐", "Weights": [10, 10, 10, 10], "Sizes": [[30, 40, 30], [30, 40, 30], [30, 40, 30], [30, 40, 130]]}
