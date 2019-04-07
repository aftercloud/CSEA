package com.spring.eduspace.controller;

import com.spring.eduspace.functions.Func;
import com.spring.eduspace.user.NewRegister;
import com.spring.eduspace.user.SysUser;
import com.spring.eduspace.user.SysUserRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.security.core.context.SecurityContextHolder;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestParam;

@Controller
public class RegisterController {

    @Autowired
    private SysUserRepository sysUserRepository;

    private static final int LINE = 2;

    @GetMapping("/registerStudent")
    public String toStudentResgisterPage(){
        return "register/registerStudent";
    }

    @GetMapping("/registerTeacher")
    public String toTeacherResgisterPage(){
        return "register/registerTeacher";
    }

    @PostMapping("/newTeacher")
    public String newTeacher(NewRegister newRegister, Model model){
        long index = 1;
        while (sysUserRepository.findById(index) != null){
            index++;
        }
        int numbers = Integer.valueOf(newRegister.getPassword());
        String[] stringList = new String[numbers];
        for(int i = 1;i <= numbers;i++){
            String username = newRegister.getUsername() + "_" +String.valueOf(i);
            SysUser user = new SysUser();
            user.setId(index++);
            user.setUsername(username);
            user.setName(username);
            user.setPassword(Func.generateRandomPassword());
            sysUserRepository.save(user);
            stringList[i-1] = user.toString();
        }
        String[] stringShow = null;
        if (numbers % LINE == 0){
            stringShow = new String[numbers/LINE];
            int iter = 0;
            for(int i=0;i<numbers;i+=LINE){
                StringBuilder builder = new StringBuilder();
                for(int j=0;j<LINE;j++){
                    builder.append(stringList[i+j]);
                }
                stringShow[iter++] = builder.toString();
            }
        }else{
            stringShow = new String[numbers/LINE + 1];
            int iter = 0;
            for(int i=0;i<numbers - LINE;i+=LINE){
                StringBuilder builder = new StringBuilder();
                for(int j=0;j<LINE;j++){
                    builder.append(stringList[i+j]);
                }
                stringShow[iter++] = builder.toString();
            }
            int last = numbers - (numbers % LINE);
            StringBuilder builder = new StringBuilder();
            for(int i=0;i<(numbers % LINE);i++){
                builder.append(stringList[last+i]);
            }
            stringShow[iter] = builder.toString();
        }
        model.addAttribute("message", stringShow);
        return "register/registerTeacherData";
    }

    @PostMapping("/newStudent")
    public String newStudent(NewRegister newRegister){
        String username = newRegister.getUsername();
        if(sysUserRepository.findByUsername(username) != null){
            return "register/registerStudent";
        }
        long index = 1;
        while (sysUserRepository.findById(index) != null){
            index++;
        }
        SysUser user = new SysUser();
        user.setId(index);
        user.setUsername(newRegister.getUsername());
        user.setName(newRegister.getUsername());
        user.setPassword(newRegister.getPassword());
        sysUserRepository.save(user);
        return "user/student";
    }

    @GetMapping("/updateName")
    public String updateName(){
        return "user/update_name";
    }

    @GetMapping("/updatePassword")
    public String updatePassword(){
        return "user/update_password";
    }

    @PostMapping("/updateName")
    public String toUpdateName(@RequestParam("name")String name){
        SysUser user = sysUserRepository.findByName(name);
        if(user != null){
            return "user/update_name";
        }
        String username = SecurityContextHolder.getContext().getAuthentication().getName();
        user = sysUserRepository.findByUsername(username);
        sysUserRepository.delete(user);
        user.setName(name);
        sysUserRepository.save(user);
        return "user/student";
    }

    @PostMapping("/updatePassword")
    public String toUpdatePassword(@RequestParam("password")String password){
        String username = SecurityContextHolder.getContext().getAuthentication().getName();
        SysUser user = sysUserRepository.findByUsername(username);
        sysUserRepository.delete(user);
        user.setPassword(password);
        sysUserRepository.save(user);
        return "user/student";
    }

}