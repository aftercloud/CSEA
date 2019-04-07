package main

import (
	"testing"
)

func TestValid(t *testing.T){
	var Tests = []struct{
		OriginLuggage
		expected string
	}{
		{OriginLuggage{Space:-5, Vip:-5, Begin:"武汉", End:"北京", Weights:[]int{10, 10}, Sizes:[][]int{[]int{20, 20, 20}, []int{20, 20, 20}}}, "VIP Error"},
		{OriginLuggage{Space:-5, Vip:8, Begin:"武汉", End:"北京", Weights:[]int{10, 10}, Sizes:[][]int{[]int{20, 20, 20}, []int{20, 20, 20}}}, "VIP Error"},
		{OriginLuggage{Space:8, Vip:0, Begin:"武汉", End:"北京", Weights:[]int{10, 10}, Sizes:[][]int{[]int{20, 20, 20}, []int{20, 20, 20}}}, "Space Error"},
		{OriginLuggage{Space:-1, Vip:0, Begin:"武汉", End:"北京", Weights:[]int{10, 10}, Sizes:[][]int{[]int{20, 20, 20}, []int{20, 20, 20}}}, "Space Error"},
		{OriginLuggage{Space:0, Vip:0, Begin:"", End:"", Weights:[]int{10, 10}, Sizes:[][]int{[]int{20, 20, 20}, []int{20, 20, 20}}}, "No Begin"},
		{OriginLuggage{Space:0, Vip:0, Begin:"武汉", End:"", Weights:[]int{10, 10}, Sizes:[][]int{[]int{20, 20, 20}, []int{20, 20, 20}}}, "No End"},
		{OriginLuggage{Space:0, Vip:0, Begin:"武汉", End:"北京", Weights:[]int{10}, Sizes:[][]int{[]int{20, 20, 20}, []int{20, 20, 20}}}, "No Size Matching"},
		{OriginLuggage{Space:0, Vip:0, Begin:"武汉", End:"北京", Weights:[]int{10}, Sizes:[][]int{[]int{20, 20}}}, "No Enough Data"},
		{OriginLuggage{Space:0, Vip:0, Begin:"武汉", End:"北京", Weights:[]int{46, 10}, Sizes:[][]int{[]int{46, 20, 20}, []int{20, 20, 20}}}, "Too Weight"},
		{OriginLuggage{Space:0, Vip:0, Begin:"武汉", End:"北京", Weights:[]int{10, 10}, Sizes:[][]int{[]int{20, 20, 20}, []int{2000, 20, 20}}}, "Beyond Size"},
		{OriginLuggage{Space:0, Vip:0, Begin:"武汉", End:"北京", Weights:[]int{-10, 10}, Sizes:[][]int{[]int{20, 20, 20}, []int{20, 20, 20}}}, "Negative Weight"},
		{OriginLuggage{Space:0, Vip:0, Begin:"武汉", End:"北京", Weights:[]int{10, 10}, Sizes:[][]int{[]int{20, 20, 20}, []int{20, 20, -20}}}, "Negative Size"},
		{OriginLuggage{Space:0, Vip:0, Begin:"武汉", End:"北京", Weights:[]int{10, 10}, Sizes:[][]int{[]int{20, 20, 20}, []int{20, -20, 20}}}, "Negative Size"},
		{OriginLuggage{Space:0, Vip:0, Begin:"武汉", End:"北京", Weights:[]int{10, 10}, Sizes:[][]int{[]int{20, 20, 20}, []int{-20, 20, 20}}}, "Negative Size"},
		{OriginLuggage{Space:0, Vip:0, Begin:"武汉", End:"北京", Weights:[]int{10, 10}, Sizes:[][]int{[]int{20, 20, 20}, []int{20, 20, 20}}}, ""},
	}
	for index, value := range Tests{
		if _, err := isLuggageValid(value.OriginLuggage);err == value.expected{
			t.Logf("Validity test case %d passed", index)
		}else{
			t.Errorf("Validity test case %d failed", index)
		}
	}
}

func TestCalc(t *testing.T){
	var Tests = []struct {
		Luggage
		expected int
	}{
		// 国内运费测试
		{Luggage{Vip:goldVip, Space:first, Begin:"武汉", End:"北京", Weights:[]int{45, 45, 45, 45}, Sizes:[]int{100, 100, 100, 100}}, 900},
		{Luggage{Vip:silverVip, Space:business, Begin:"武汉", End:"北京", Weights:[]int{40, 40}, Sizes:[]int{100, 100}}, 0},
		{Luggage{Vip:normalVIP, Space:bright, Begin:"武汉", End:"北京", Weights:[]int{21}, Sizes:[]int{100}}, 15},
		{Luggage{Vip:normalVIP, Space:economy, Begin:"武汉", End:"北京", Weights:[]int{21}, Sizes:[]int{100}}, 15},
		{Luggage{Vip:noVIP, Space:baby, Begin:"武汉", End:"北京", Weights:[]int{40, 40}, Sizes:[]int{100, 100}}, 1050},
		// 区域一运费测试
		{Luggage{Vip:noVIP, Space:first, Begin:"武汉", End:"日本", Weights:[]int{32, 32, 32}, Sizes:[]int{100, 100, 100}}, 0},
		{Luggage{Vip:noVIP, Space:first, Begin:"武汉", End:"日本", Weights:[]int{32, 32, 10, 10}, Sizes:[]int{100, 100, 100, 100}}, 1000},
		{Luggage{Vip:noVIP, Space:business, Begin:"武汉", End:"日本", Weights:[]int{32, 33}, Sizes:[]int{200, 100}}, 4000},
		{Luggage{Vip:normalVIP, Space:bright, Begin:"武汉", End:"日本", Weights:[]int{23, 23, 23, 23, 23}, Sizes:[]int{100, 100, 100, 100, 100}}, 3000},
		{Luggage{Vip:silverVip, Space:economy, Begin:"日本", End:"武汉", Weights:[]int{24, 33, 23}, Sizes:[]int{100, 100, 200}}, 5000},
		{Luggage{Vip:goldVip, Space:baby, Begin:"武汉", End:"日本", Weights:[]int{23, 23, 24, 33}, Sizes:[]int{100, 100, 200, 100}}, 8000},
		// 区域二运费测试
		{Luggage{Vip:noVIP, Space:first, Begin:"武汉", End:"格鲁吉亚", Weights:[]int{32, 32, 32}, Sizes:[]int{100, 100, 100}}, 0},
		{Luggage{Vip:noVIP, Space:first, Begin:"武汉", End:"格鲁吉亚", Weights:[]int{32, 32, 32, 32}, Sizes:[]int{100, 100, 100, 100}}, 450},
		{Luggage{Vip:noVIP, Space:business, Begin:"武汉", End:"格鲁吉亚", Weights:[]int{32, 32, 32, 32}, Sizes:[]int{100, 100, 100, 100}}, 1750},
		{Luggage{Vip:normalVIP, Space:bright, Begin:"武汉", End:"格鲁吉亚", Weights:[]int{33, 33}, Sizes:[]int{200, 159}}, 8000},
		{Luggage{Vip:silverVip, Space:economy, Begin:"格鲁吉亚", End:"武汉", Weights:[]int{32, 33, 10, 10}, Sizes:[]int{200, 100, 100, 100}}, 5750},
		{Luggage{Vip:goldVip, Space:baby, Begin:"武汉", End:"格鲁吉亚", Weights:[]int{32, 33, 10, 10}, Sizes:[]int{200, 100, 100, 100}}, 8750},
		// 区域三运费测试
		{Luggage{Vip:noVIP, Space:first, Begin:"武汉", End:"内罗毕", Weights:[]int{32, 32, 32}, Sizes:[]int{100, 100, 100}}, 0},
		{Luggage{Vip:noVIP, Space:first, Begin:"武汉", End:"内罗毕", Weights:[]int{32, 32, 32, 32}, Sizes:[]int{100, 100, 100, 100}}, 1000},
		{Luggage{Vip:noVIP, Space:business, Begin:"武汉", End:"内罗毕", Weights:[]int{32, 33}, Sizes:[]int{200, 100}}, 4000},
		{Luggage{Vip:normalVIP, Space:bright, Begin:"武汉", End:"内罗毕", Weights:[]int{23, 23, 23, 23, 23}, Sizes:[]int{100, 100, 100, 100, 100}}, 3000},
		{Luggage{Vip:silverVip, Space:economy, Begin:"内罗毕", End:"武汉", Weights:[]int{32, 33}, Sizes:[]int{200, 100}}, 6000},
		{Luggage{Vip:goldVip, Space:baby, Begin:"武汉", End:"内罗毕", Weights:[]int{32, 33, 10, 10}, Sizes:[]int{200, 100, 100, 100}}, 9000},
		// 区域四运费测试
		{Luggage{Vip:noVIP, Space:business, Begin:"武汉", End:"艾尔", Weights:[]int{23, 23}, Sizes:[]int{100, 100}}, 0},
		{Luggage{Vip:noVIP, Space:first, Begin:"武汉", End:"艾尔", Weights:[]int{32, 32, 32, 32}, Sizes:[]int{100, 100, 100, 100}}, 450},
		{Luggage{Vip:noVIP, Space:first, Begin:"武汉", End:"艾尔", Weights:[]int{32, 33, 23}, Sizes:[]int{200, 100, 100}}, 4000},
		{Luggage{Vip:normalVIP, Space:bright, Begin:"武汉", End:"艾尔", Weights:[]int{23, 23, 23, 23, 23}, Sizes:[]int{100, 100, 100, 100, 100}}, 1750},
		{Luggage{Vip:silverVip, Space:economy, Begin:"艾尔", End:"武汉", Weights:[]int{32, 33}, Sizes:[]int{200, 100}}, 5000},
		{Luggage{Vip:silverVip, Space:business, Begin:"艾尔", End:"武汉", Weights:[]int{23, 23, 23, 23}, Sizes:[]int{200, 100, 100, 100}}, 1000},
		{Luggage{Vip:goldVip, Space:baby, Begin:"武汉", End:"艾尔", Weights:[]int{32, 33, 10, 10, 23}, Sizes:[]int{200, 100, 100, 100, 100}}, 8050},
		// 特殊情况运费测试
		{Luggage{Begin:"新加坡", End:"武汉", Weights:[]int{10, 10, 10, 10}, Sizes:[]int{158, 100, 100, 159}}, 2000},
		{Luggage{Space:economy, Begin:"兰州", End:"迪拜", Weights:[]int{10, 10, 10, 10}, Sizes:[]int{100, 100, 100, 200}}, 6000},
		{Luggage{Space:bright, Begin:"乌鲁木齐", End:"迪拜", Weights:[]int{10, 10, 10, 10}, Sizes:[]int{100, 100, 100, 200}}, 6000},
		{Luggage{Space:first, Begin:"迪拜", End:"乌鲁木齐", Weights:[]int{10, 10, 10, 10}, Sizes:[]int{100, 100, 100, 200}}, 2000},
		{Luggage{Space:business, Begin:"迪拜", End:"兰州", Weights:[]int{10, 10, 10, 10}, Sizes:[]int{100, 100, 100, 200}}, 4000},
	}
	for index, value := range Tests{
		if calc(value.Luggage) == value.expected{
			t.Logf("Serviceability test case %d passed", index)
		}else{
			t.Errorf("Serviceability test case %d failed", index)
		}
	}
}
