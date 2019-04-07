package com.spring.eduspace.controller;

import com.spring.eduspace.user.NewRegister;
import com.spring.eduspace.user.SysUser;
import com.spring.eduspace.user.SysUserRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.security.core.context.SecurityContextHolder;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestMapping;

@Controller
public class IndexController {

    @Autowired
    SysUserRepository sysUserRepository;

    @GetMapping("/")
    public String iLogin() {
        return "login";
    }

    @GetMapping("/login")
    public String login() {
        return "login";
    }

    @PostMapping("/hello")
    public String loginSuccess(Model model) {
        SysUser sysUser = sysUserRepository.findByUsername(SecurityContextHolder.getContext().getAuthentication().getName());
        model.addAttribute("name", sysUser.getName());
        String authorities = SecurityContextHolder.getContext().getAuthentication().getAuthorities().toString();
        String teacher = "[ROLE_ADMIN]";
        if(authorities.equals(teacher)){
            return "user/teacher";
        }
        return "user/student";
    }

    @PostMapping("/teacher")
    public String toTeacher(Model model) {
        SysUser sysUser = sysUserRepository.findByUsername(SecurityContextHolder.getContext().getAuthentication().getName());
        model.addAttribute("name", sysUser.getName());
        return "user/teacher";
    }

    @GetMapping("/teacher")
    public String getToTeacher(Model model) {
        SysUser sysUser = sysUserRepository.findByUsername(SecurityContextHolder.getContext().getAuthentication().getName());
        model.addAttribute("name", sysUser.getName());
        return "user/teacher";
    }

    @PostMapping("/student")
    public String toStudent(Model model) {
        SysUser sysUser = sysUserRepository.findByUsername(SecurityContextHolder.getContext().getAuthentication().getName());
        model.addAttribute("name", sysUser.getName());
        return "user/student";
    }

    @GetMapping("/student")
    public String getToStudent(Model model) {
        SysUser sysUser = sysUserRepository.findByUsername(SecurityContextHolder.getContext().getAuthentication().getName());
        model.addAttribute("name", sysUser.getName());
        return "user/student";
    }

}