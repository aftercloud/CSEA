package com.spring.eduspace.controller;

import com.spring.eduspace.functions.Func;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.multipart.MultipartFile;

import java.io.File;
import java.io.FileInputStream;

@Controller
public class CodeController {

    @RequestMapping("/code_jdbc")
    public String codeJDBC(Model model){
        File file = new File("");
        String filePath = file.getAbsolutePath() + "\\classes\\static\\code\\JDBC.txt";
        file = new File(filePath);
        Long fileLength = file.length();
        byte[] bytes = new byte[fileLength.intValue()];
        try {
            FileInputStream in = new FileInputStream(file);
            in.read(bytes);
            in.close();
            model.addAttribute("code", new String(bytes));
        }
        catch (Exception e){
            System.out.println(e.toString());
        }
        return "code/code_jdbc";
    }

    @RequestMapping("/code_jsp")
    public String codeJSP(Model model){
        File file = new File("");
        String filePath = file.getAbsolutePath() + "\\classes\\static\\code\\JSP.txt";
        file = new File(filePath);
        Long fileLength = file.length();
        byte[] bytes = new byte[fileLength.intValue()];
        try {
            FileInputStream in = new FileInputStream(file);
            in.read(bytes);
            in.close();
            model.addAttribute("code", new String(bytes));
        }
        catch (Exception e){
            System.out.println(e.toString());
        }
        return "code/code_jsp";
    }

    @RequestMapping("/code_servlet")
    public String codeServlet(Model model){
        File file = new File("");
        String filePath = file.getAbsolutePath() + "\\classes\\static\\code\\Servlet.txt";
        file = new File(filePath);
        Long fileLength = file.length();
        byte[] bytes = new byte[fileLength.intValue()];
        try {
            FileInputStream in = new FileInputStream(file);
            in.read(bytes);
            in.close();
            model.addAttribute("code", new String(bytes));
        }
        catch (Exception e){
            System.out.println(e.toString());
        }
        return "code/code_servlet";
    }

    @RequestMapping("/code_springboot")
    public String codeSpringBoot(Model model){
        File file = new File("");
        String filePath = file.getAbsolutePath() + "\\classes\\static\\code\\SpringBoot.txt";
        file = new File(filePath);
        Long fileLength = file.length();
        byte[] bytes = new byte[fileLength.intValue()];
        try {
            FileInputStream in = new FileInputStream(file);
            in.read(bytes);
            in.close();
            model.addAttribute("code", new String(bytes));
        }
        catch (Exception e){
            System.out.println(e.toString());
        }
        return "code/code_springboot";
    }

    @PostMapping("/sava_jdbc")
    public String saveJDBC(@RequestParam("file") MultipartFile file, Model model){
        File f = new File("");
        String filePath = f.getAbsolutePath() + "\\classes\\static\\code\\";
        try {
            Func.uploadFile(file.getBytes(), filePath, "JDBC.txt");
        }
        catch (Exception e){
            System.out.println(e.toString());
        }
        {
            f = new File("");
            filePath = f.getAbsolutePath() + "\\classes\\static\\code\\JDBC.txt";
            f = new File(filePath);
            Long fileLength = f.length();
            byte[] bytes = new byte[fileLength.intValue()];
            try {
                FileInputStream in = new FileInputStream(f);
                in.read(bytes);
                in.close();
                model.addAttribute("code", new String(bytes));
            }
            catch (Exception e){
                System.out.println(e.toString());
            }
        }
        return "code/code_jdbc";
    }

    @PostMapping("/sava_jsp")
    public String saveJSP(@RequestParam("file") MultipartFile file, Model model){
        File f = new File("");
        String filePath = f.getAbsolutePath() + "\\classes\\static\\code\\";
        try {
            Func.uploadFile(file.getBytes(), filePath, "JSP.txt");
        }
        catch (Exception e){
            System.out.println(e.toString());
        }
        {
            f = new File("");
            filePath = f.getAbsolutePath() + "\\classes\\static\\code\\JSP.txt";
            f = new File(filePath);
            Long fileLength = f.length();
            byte[] bytes = new byte[fileLength.intValue()];
            try {
                FileInputStream in = new FileInputStream(f);
                in.read(bytes);
                in.close();
                model.addAttribute("code", new String(bytes));
            }
            catch (Exception e){
                System.out.println(e.toString());
            }
        }
        return "code/code_jsp";
    }

    @PostMapping("/sava_serlvet")
    public String saveServlet(@RequestParam("file") MultipartFile file, Model model){
        File f = new File("");
        String filePath = f.getAbsolutePath() + "\\classes\\static\\code\\";
        try {
            Func.uploadFile(file.getBytes(), filePath, "Servlet.txt");
        }
        catch (Exception e){
            System.out.println(e.toString());
        }
        {
            f = new File("");
            filePath = f.getAbsolutePath() + "\\classes\\static\\code\\Servlet.txt";
            f = new File(filePath);
            Long fileLength = f.length();
            byte[] bytes = new byte[fileLength.intValue()];
            try {
                FileInputStream in = new FileInputStream(f);
                in.read(bytes);
                in.close();
                model.addAttribute("code", new String(bytes));
            }
            catch (Exception e){
                System.out.println(e.toString());
            }
        }
        return "code/code_servlet";
    }

    @PostMapping("/sava_springboot")
    public String saveSpringBoot(@RequestParam("file") MultipartFile file, Model model){
        File f = new File("");
        String filePath = f.getAbsolutePath() + "\\classes\\static\\code\\";
        try {
            Func.uploadFile(file.getBytes(), filePath, "SpringBoot.txt");
        }
        catch (Exception e){
            System.out.println(e.toString());
        }
        {
            f = new File("");
            filePath = f.getAbsolutePath() + "\\classes\\static\\code\\SpringBoot.txt";
            f = new File(filePath);
            Long fileLength = f.length();
            byte[] bytes = new byte[fileLength.intValue()];
            try {
                FileInputStream in = new FileInputStream(f);
                in.read(bytes);
                in.close();
                model.addAttribute("code", new String(bytes));
            }
            catch (Exception e){
                System.out.println(e.toString());
            }
        }
        return "code/code_springboot";
    }

}
