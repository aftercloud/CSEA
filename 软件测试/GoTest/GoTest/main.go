package main

import (
	"encoding/json"
	"fmt"
	"net/http"
	"strconv"
	"strings"
)

// VIP
const(
	noVIP = iota
	normalVIP
	silverVip
	goldVip
)

// Area
const(
	area0 = iota
	area1
	area2
	area3
	area4
)

// Country/City
const(
	c0 = "乌鲁木齐/兰州/北京/武汉"
	c1 = "日本/美洲/澳洲/俄罗斯/迪拜"
	c2 = "乌兹别克斯坦/塔吉克斯坦/哈萨克斯坦/吉尔吉斯斯坦/土库曼斯坦/伊朗/巴基斯坦/阿塞拜疆/格鲁吉亚"
	c3 = "内罗毕/韩国"
	c4 = "新加坡/毛里求斯"
)

// Space
const(
	first = iota
	business
	bright
	economy
	baby
)

type Luggage struct {
	Area int // 航线
	Space int // 舱位
	Vip int // VIP等级
	Begin string // 出发地
	End string // 到达地
	Weights []int // 重量 切片
	Sizes []int // 尺寸和 切片
}

type OriginLuggage struct {
	Space int // 舱位
	Vip int // VIP等级
	Begin string // 出发地
	End string // 到达地
	Weights []int // 重量 切片
	Sizes [][]int // 二维尺寸 切片
}

type Limit struct {
	number int // 免费行李件数上限
	weight int // 单件免费行李重量上限
	size int // 单件免费行李尺寸上限
}

type OverWeight struct {
	number int // 超额行李件数
	weights []int // 实际行李重量与免费额度的差值
	sizes []int // 实际行李尺寸与免费额度的差值
}

// 国内航班的免费额度
func newLimitArea0(luggage Luggage, limit Limit) Limit {
	limit.size = 158
	switch luggage.Vip {
	case silverVip:
		limit.weight += 10
		limit.number += 1
	case goldVip:
		limit.weight += 20
		limit.number += 1
	}
	switch luggage.Space {
	case first:
		limit.weight += 40
		limit.number += 1
	case business:
		limit.weight += 30
		limit.number += 1
	case bright:
		fallthrough
	case economy:
		limit.weight += 20
		limit.number += 1
	case baby:
		limit.weight += 10
		limit.number += 1
	}
	return limit
}

// 区域一航班的免费额度
func newLimitArea1(luggage Luggage, limit Limit) Limit {
	limit.size = 158
	switch luggage.Vip {
	case normalVIP:
		limit.number += 1
	case silverVip:
		limit.number += 1
		limit.weight += 10
	case goldVip:
		limit.number += 1
		limit.weight += 20
	}
	switch luggage.Space {
	case first:
		limit.weight += 32
		limit.number += 3
	case business:
		limit.weight += 32
		limit.number += 2
	case bright:
		fallthrough
	case economy:
		limit.weight += 23
		limit.number += 2
	case baby:
		limit.weight += 23
		limit.number += 1
	}
	return limit
}

// 区域二航班的免费额度
func newLimitArea2(luggage Luggage, limit Limit) Limit {
	limit.size = 158
	switch luggage.Vip {
	case normalVIP:
		limit.number += 1
	case silverVip:
		limit.number += 1
	case goldVip:
		limit.number += 1
	}
	switch luggage.Space {
	case first:
		limit.weight += 32
		limit.number += 3
	case business:
		limit.weight += 32
		limit.number += 2
	case bright:
		fallthrough
	case economy:
		limit.weight += 32
		limit.number += 1
	case baby:
		limit.weight += 23
		limit.number += 1
	}
	return limit
}

// 区域三航班的免费额度
func newLimitArea3(luggage Luggage, limit Limit) Limit {
	limit.size = 158
	switch luggage.Vip {
	case normalVIP:
		limit.number += 1
	case silverVip:
		limit.number += 1
	case goldVip:
		limit.number += 1
	}
	switch luggage.Space {
	case first:
		limit.weight += 32
		limit.number += 3
	case business:
		limit.weight += 32
		limit.number += 2
	case bright:
		fallthrough
	case economy:
		limit.weight += 23
		limit.number += 2
	case baby:
		limit.weight += 23
		limit.number += 1
	}
	return limit
}

// 区域四航班的免费额度
func newLimitArea4(luggage Luggage, limit Limit) Limit {
	limit.size = 158
	switch luggage.Vip {
	case normalVIP:
		limit.number += 1
	case silverVip:
		limit.number += 1
	case goldVip:
		limit.number += 1
	}
	switch luggage.Space {
	case first:
		limit.weight += 32
		limit.number += 3
	case business:
		limit.weight += 23
		limit.number += 3
	case bright:
		limit.weight += 23
		limit.number += 2
	case economy:
		limit.weight += 23
		limit.number += 1
	case baby:
		limit.weight += 23
		limit.number += 1
	}
	return limit
}

// 行李是否超出免费额度
func isOverWeight(luggage Luggage, limit Limit) bool {
	if len(luggage.Weights) > limit.number{
		return true
	}
	for _, v := range luggage.Weights{
		if v > limit.weight{
			return true
		}
	}
	if limit.size > 0{
		for _, v := range luggage.Sizes{
			if v > limit.size{
				return true
			}
		}
	}
	return false
}

// 获得超出的额度
func getOverWeight(luggage Luggage, limit Limit)OverWeight {
	var overLimit OverWeight
	overLimit.number = len(luggage.Weights) - limit.number
	for i:=0;i<len(luggage.Weights);i++{
		overLimit.weights = append(overLimit.weights, luggage.Weights[i]-limit.weight)
		overLimit.sizes = append(overLimit.sizes, luggage.Sizes[i]-limit.size)
	}
	return overLimit
}

// 计算国内航班行李托运费用
func calcArea0(luggage Luggage, limit Limit) int {
	var fee int
	overWeight := -1 * limit.number * limit.weight
	for _, v := range luggage.Weights{
		overWeight += v
	}
	if overWeight > 0 {
		fee = overWeight * 15
	}
	return fee
}

// 计算区域一航班行李托运费用
func calcArea1(luggage Luggage, limit Limit) int {
	var fee int
	if isOverWeight(luggage, limit){
		overWeight := getOverWeight(luggage, limit)
		switch luggage.Space {
		case first:fallthrough
		case business:
			if overWeight.number > 0 {
				fee += 2000 * overWeight.number - 1000
			}
			for _, v := range overWeight.weights{
				if v > 0 {
					fee += 3000
				}
			}
			for _, v := range overWeight.sizes{
				if v > 0 {
					fee += 1000
				}
			}
		case bright:fallthrough
		case economy:fallthrough
		case baby:
			if overWeight.number > 0 {
				fee += 2000 * overWeight.number - 1000
			}
			for _, v := range overWeight.sizes{
				if v > 0 {
					fee += 1000
				}
			}
			for _, v := range overWeight.weights{
				if v > 9 {
					fee += 3000
				}else if v > 0{
					fee += 1000
				}
			}
		}
	}
	return fee
}

// 计算区域二航班行李托运费用
func calcArea2(luggage Luggage, limit Limit) int {
	var fee int
	if isOverWeight(luggage, limit){
		overWeight := getOverWeight(luggage, limit)
		switch luggage.Space {
		case first:fallthrough
		case business:fallthrough
		case bright:fallthrough
		case economy:fallthrough
		case baby:
			if overWeight.number > 0 {
				fee += 1300 * (overWeight.number - 1) + 450
			}
			for _, v := range overWeight.weights{
				if v > 0 {
					fee += 3000
				}
			}
			for _, v := range overWeight.sizes{
				if v > 0 {
					fee += 1000
				}
			}
		}
	}
	return fee
}

// 计算区域三航班行李托运费用
func calcArea3(luggage Luggage, limit Limit) int {
	var fee int
	if isOverWeight(luggage, limit){
		overWeight := getOverWeight(luggage, limit)
		switch luggage.Space {
		case first:fallthrough
		case business:
			if overWeight.number > 0 {
				fee += 2000 * overWeight.number - 1000
			}
			for _, v := range overWeight.weights{
				if v > 0 {
					fee += 3000
				}
			}
			for _, v := range overWeight.sizes{
				if v > 0 {
					fee += 1000
				}
			}
		case bright:fallthrough
		case economy:fallthrough
		case baby:
			if overWeight.number > 0 {
				fee += 2000 * overWeight.number - 1000
			}
			for _, v := range overWeight.sizes{
				if v > 0 {
					fee += 1000
				}
			}
			for _, v := range overWeight.weights{
				if v > 9 {
					fee += 3000
				}else if v > 0{
					fee += 2000
				}
			}
		}
	}
	return fee
}

// 计算区域四航班行李托运费用
func calcArea4(luggage Luggage, limit Limit) int {
	var fee int
	if isOverWeight(luggage, limit){
		overWeight := getOverWeight(luggage, limit)
		switch luggage.Space {
		case first:
			if overWeight.number > 0 {
				fee += 1300 * (overWeight.number - 1) + 450
			}
			for _, v := range overWeight.weights{
				if v > 0 {
					fee += 3000
				}
			}
			for _, v := range overWeight.sizes{
				if v > 0 {
					fee += 1000
				}
			}
		case business:fallthrough
		case bright:fallthrough
		case economy:fallthrough
		case baby:
			if overWeight.number > 0 {
				fee += 1300 * (overWeight.number - 1) + 450
			}
			for _, v := range overWeight.sizes{
				if v > 0 {
					fee += 1000
				}
			}
			for _, v := range overWeight.weights{
				if v > 9 {
					fee += 3000
				}else if v > 0{
					fee += 1000
				}
			}
		}
	}
	return fee
}

// 判断特殊情况
func specialType(luggage Luggage) (mark int) {
	if (strings.Contains(luggage.Begin, "兰州") || strings.Contains(luggage.Begin, "乌鲁木齐")) && strings.Contains(luggage.End, "迪拜"){
		return 1
	}
	if (strings.Contains(luggage.End, "兰州") || strings.Contains(luggage.End, "乌鲁木齐")) && strings.Contains(luggage.Begin, "迪拜"){
		return 1
	}
	if strings.Contains(luggage.Begin, "新加坡"){
		return 2
	}
	return 
}

// 计算特殊情况下的免费额度
func newSpecialLimit(luggage Luggage, mark int) (newLuggage Luggage, limit Limit) {
	newLuggage = luggage
	switch mark {
	case 1:
		if luggage.Space == bright || luggage.Space == economy{
			limit.number -= 1
			if luggage.Space == bright{
				limit.weight += 9
			}
		}
	case 2:
		newLuggage.Area = area1
	}
	return 
}

// 填充航线信息
func generateLuggage(stringLuggage Luggage) (luggage Luggage){
	luggage = stringLuggage
	if strings.Contains(c1, luggage.Begin) || strings.Contains(c1, luggage.End){
		luggage.Area = area1
	}else if strings.Contains(c2, luggage.Begin) || strings.Contains(c2, luggage.End){
		luggage.Area = area2
	}else if strings.Contains(c3, luggage.Begin) || strings.Contains(c3, luggage.End){
		luggage.Area = area3
	}else if strings.Contains(c0, luggage.Begin) && strings.Contains(c0, luggage.End){
		luggage.Area = area0
	}else{
		luggage.Area = area4
	}
	return
}

// 计算托运费用
func calc(luggage Luggage) int {
	var fee int
	var limit Limit
	luggage = generateLuggage(luggage)
	sType := specialType(luggage)
	if sType > 0{
		luggage, limit = newSpecialLimit(luggage, sType)
	}
	switch luggage.Area {
		case area0:
			limit = newLimitArea0(luggage, limit)
			fee = calcArea0(luggage, limit)
		case area1:
			limit = newLimitArea1(luggage, limit)
			fee = calcArea1(luggage, limit)
		case area2:
			limit = newLimitArea2(luggage, limit)
			fee = calcArea2(luggage, limit)
		case area3:
			limit = newLimitArea3(luggage, limit)
			fee = calcArea3(luggage, limit)
		case area4:
			limit = newLimitArea4(luggage, limit)
			fee = calcArea4(luggage, limit)
	}
	return fee
}

// 判断原始信息是否合法
func isLuggageValid(luggage OriginLuggage) (bool, string) {
	if luggage.Vip < 0 || luggage.Vip > 3{
		return false, "VIP Error"
	}
	if luggage.Space < 0 || luggage.Space > 4 {
		return false, "Space Error"
	}
	if len(luggage.Begin) == 0 {
		return false, "No Begin"
	}
	if len(luggage.End) == 0 {
		return false, "No End"
	}
	if len(luggage.Weights) != len(luggage.Sizes){
		return false, "No Size Matching"
	}
	for _, v := range luggage.Sizes{
		if len(v) != 3 {
			return false, "No Enough Data"
		}
	}
	for _, v := range luggage.Weights{
		if v > 45 {
			return false, "Too Weight"
		}else if v <= 0 {
			return false, "Negative Weight"
		}
	}
	for _, v := range luggage.Sizes{
		if v[0] + v[1] + v[2] > 300{
			return false, "Beyond Size"
		}else if v[0] <= 0 || v[1] <= 0 || v[2] <= 0 {
			return false, "Negative Size"
		}
	}
	return true, ""
}

// 原始信息转行李信息
func origin2Luggage(o OriginLuggage) (l Luggage){
	l.Weights = o.Weights
	l.Begin = o.Begin
	l.End = o.End
	l.Space = o.Space
	l.Vip = o.Vip
	for _, v := range o.Sizes{
		l.Sizes = append(l.Sizes, v[0] + v[1] + v[2])
	}
	return
}

// 封装上述全部过程
func calcFee(o OriginLuggage) int {
	var fee = -1
	if ok, _ := isLuggageValid(o); ok{
		luggage := origin2Luggage(o)
		fee = calc(luggage)
	}
	return fee
}

// 模块结束

// 启动服务
func handler(writer http.ResponseWriter, request *http.Request){
	var fee int
	var o OriginLuggage
	err := request.ParseForm()
	if err != nil {
		err.Error()
	}else{
		if len(request.Form) > 0 {
			for _, v := range request.Form{
				v[0] = strings.Replace(v[0], "'", "\"", -1)
				err = json.Unmarshal([]byte(v[0]), &o)
			}
			if err != nil {
				fee = -1
			}else{
				fee = calcFee(o)
			}
		}
	}
	fmt.Println(calcFee(o))
	_, err = fmt.Fprint(writer, strconv.Itoa(fee))
	if err != nil {
		err.Error()
	}
}

// 开启服务
func main(){
	http.HandleFunc("/", handler)
	err := http.ListenAndServe(":8080", nil)
	if err != nil {
		err.Error()
	}

}
