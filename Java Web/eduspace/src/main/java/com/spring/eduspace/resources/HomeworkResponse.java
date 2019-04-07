package com.spring.eduspace.resources;

public class HomeworkResponse {
    private String p0 = null;
    private String p1 = null;
    private String p2 = null;
    private String p3 = null;
    private String p4 = null;
    private String p5 = null;
    private String p6 = null;
    private String p7 = null;
    private String p8 = null;
    private String p9 = null;

    private String[] p = new String[10];

    public void setP0(String p0) {
        this.p0 = p0;
    }

    public void setP1(String p1) {
        this.p1 = p1;
    }

    public void setP2(String p2) {
        this.p2 = p2;
    }

    public void setP3(String p3) {
        this.p3 = p3;
    }

    public void setP4(String p4) {
        this.p4 = p4;
    }

    public void setP5(String p5) {
        this.p5 = p5;
    }

    public void setP6(String p6) {
        this.p6 = p6;
    }

    public void setP7(String p7) {
        this.p7 = p7;
    }

    public void setP8(String p8) {
        this.p8 = p8;
    }

    public void setP9(String p9) {
        this.p9 = p9;
    }

    public void setP(String[] p) {
        this.p = p;
    }

    public String getP0() {
        return p0;
    }

    public String getP1() {
        return p1;
    }

    public String getP2() {
        return p2;
    }

    public String getP3() {
        return p3;
    }

    public String getP4() {
        return p4;
    }

    public String getP5() {
        return p5;
    }

    public String getP6() {
        return p6;
    }

    public String getP7() {
        return p7;
    }

    public String getP8() {
        return p8;
    }

    public String getP9() {
        return p9;
    }

    public String[] getP() {
        return p;
    }

    public void change(){
        p[0] = p0;
        p[1] = p1;
        p[2] = p2;
        p[3] = p3;
        p[4] = p4;
        p[5] = p5;
        p[6] = p6;
        p[7] = p7;
        p[8] = p8;
        p[9] = p9;
    }

    public int count(){
        if(p0 == null){
            return 0;
        }
        if(p1 == null){
            return 1;
        }
        if(p2 == null){
            return 2;
        }
        if(p3 == null){
            return 3;
        }
        if(p4 == null){
            return 4;
        }
        if(p5 == null){
            return 5;
        }
        if(p6 == null){
            return 6;
        }
        if(p7 == null){
            return 7;
        }
        if(p8 == null){
            return 8;
        }
        if(p9 == null){
            return 9;
        }
        return 10;
    }

}
