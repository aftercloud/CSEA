package com.spring.eduspace.controller;

import com.spring.eduspace.functions.Func;
import com.spring.eduspace.resources.PdfInfo;
import com.spring.eduspace.resources.PdfInfoRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;

import java.util.List;

@Controller
public class DownloadController {

    @Autowired
    PdfInfoRepository pdfInfoRepository;

    @RequestMapping("/download_jdbc")
    public String downloadJDBC(Model model){
        int type = 1;
        List<PdfInfo>pdfInfos = pdfInfoRepository.findAll();
        int length = pdfInfos.size();
        int count = 0;
        for(int i=0;i<length;i++){
            PdfInfo pdfInfo = pdfInfos.get(i);
            if (pdfInfo.getType() == type){
                count++;
            }
        }
        PdfInfo[] infos = new PdfInfo[count];
        count = 0;
        for(int i=0;i<length;i++){
            PdfInfo pdfInfo = pdfInfos.get(i);
            if (pdfInfo.getType() == type){
                infos[count++] = pdfInfo;
            }
        }
        model.addAttribute("info", infos);
        return "download/download_jdbc";
    }

    @RequestMapping("/download_jsp")
    public String downloadJSP(Model model){
        int type = 2;
        List<PdfInfo>pdfInfos = pdfInfoRepository.findAll();
        int length = pdfInfos.size();
        int count = 0;
        for(int i=0;i<length;i++){
            PdfInfo pdfInfo = pdfInfos.get(i);
            if (pdfInfo.getType() == type){
                count++;
            }
        }
        PdfInfo[] infos = new PdfInfo[count];
        count = 0;
        for(int i=0;i<length;i++){
            PdfInfo pdfInfo = pdfInfos.get(i);
            if (pdfInfo.getType() == type){
                infos[count++] = pdfInfo;
            }
        }
        model.addAttribute("info", infos);
        return "download/download_jsp";
    }

    @RequestMapping("/download_servlet")
    public String downloadServlet(Model model){
        int type = 3;
        List<PdfInfo>pdfInfos = pdfInfoRepository.findAll();
        int length = pdfInfos.size();
        int count = 0;
        for(int i=0;i<length;i++){
            PdfInfo pdfInfo = pdfInfos.get(i);
            if (pdfInfo.getType() == type){
                count++;
            }
        }
        PdfInfo[] infos = new PdfInfo[count];
        count = 0;
        for(int i=0;i<length;i++){
            PdfInfo pdfInfo = pdfInfos.get(i);
            if (pdfInfo.getType() == type){
                infos[count++] = pdfInfo;
            }
        }
        model.addAttribute("info", infos);
        return "download/download_servlet";
    }

    @RequestMapping("/download_springboot")
    public String downloadSpringBoot(Model model){
        int type = 4;
        List<PdfInfo>pdfInfos = pdfInfoRepository.findAll();
        int length = pdfInfos.size();
        int count = 0;
        for(int i=0;i<length;i++){
            PdfInfo pdfInfo = pdfInfos.get(i);
            if (pdfInfo.getType() == type){
                count++;
            }
        }
        PdfInfo[] infos = new PdfInfo[count];
        count = 0;
        for(int i=0;i<length;i++){
            PdfInfo pdfInfo = pdfInfos.get(i);
            if (pdfInfo.getType() == type){
                infos[count++] = pdfInfo;
            }
        }
        model.addAttribute("info", infos);
        return "download/download_springboot";
    }

    @PostMapping("/pre_show")
    public String preShow(@RequestParam("Id")long Id, Model model){
        PdfInfo pdfInfo = pdfInfoRepository.findById(Id);
        String f = "/pdf/";
        if(pdfInfo.getType() == Func.JDBCKEY){
            f += "JDBC/";
        }else if(pdfInfo.getType() == Func.JSPKEY){
            f += "JSP/";
        }else if(pdfInfo.getType() == Func.SERVLETKEY){
            f += "Servlet/";
        }else{
            f += "SpringBoot/";
        }
        f += pdfInfo.getFileName();
        model.addAttribute("file", f);
        return "download/download_pre_show";
    }

}
