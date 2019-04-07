package com.spring.eduspace.controller;

import com.spring.eduspace.functions.Func;
import com.spring.eduspace.resources.*;
import com.spring.eduspace.user.SysUser;
import com.spring.eduspace.user.SysUserRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.security.core.context.SecurityContextHolder;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.*;
import org.springframework.web.multipart.MultipartFile;

import javax.servlet.http.HttpServletResponse;
import java.io.*;
import java.util.List;

@Controller
public class HomeworkController {

    @Autowired
    private SysUserRepository sysUserRepository;

    @Autowired
    private ExerciseRepository exerciseRepository;

    @Autowired
    private HomeworkRepository homeworkRepository;

    @GetMapping("/homework")
    public String toHomework(Model model){
        List<Homework>homework = homeworkRepository.findAll();
        model.addAttribute("message", homework);
        return "homework/homework";
    }

    @GetMapping("/exercise")
    public String exercise(Model model) {
        List<Exercise>exercises = exerciseRepository.findAll();
        model.addAttribute("message", exercises);
        return "homework/exercise";
    }

    @RequestMapping(value = "/de/{Id}")
    public void de(@PathVariable("Id") String Id, HttpServletResponse res){
        File f = new File("");
        String path = f.getAbsolutePath() + "\\classes\\static\\homework\\";
        Exercise exercise = exerciseRepository.findById(Long.valueOf(Id).longValue());
        res.setHeader("content-type", "application/octet-stream");
        res.setContentType("application/octet-stream");
        res.setHeader("Content-Disposition", "attachment;filename=" + exercise.getFileName());
        byte[] buff = new byte[1024];
        BufferedInputStream bis = null;
        OutputStream os = null;
        try {
            os = res.getOutputStream();
            bis = new BufferedInputStream(new FileInputStream(new File(path + exercise.getFileName())));
            int i = bis.read(buff);
            while (i != -1) {
                os.write(buff, 0, buff.length);
                os.flush();
                i = bis.read(buff);
            }
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            if (bis != null) {
                try {
                    bis.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    @GetMapping("/homework_s")
    public String toHomeworkS(Model model) {
        File f = new File("");
        String filePath = f.getAbsolutePath() + "\\classes\\static\\homework\\homework.txt";
        try{
            FileInputStream fis = new FileInputStream(filePath);
            InputStreamReader is = new InputStreamReader(fis, "UTF-8");
            BufferedReader br = new BufferedReader(is);
            model.addAttribute("title", br.readLine());
            model.addAttribute("date", br.readLine());
            int num = Integer.valueOf(br.readLine());
            String[] problems = new String[num];
            for (int i=0;i<num;i++){
                problems[i] = br.readLine();
            }
            model.addAttribute("problems", problems);
            SysUser user = sysUserRepository.findByUsername(SecurityContextHolder.getContext().getAuthentication().getName());
            Homework homework = homeworkRepository.findById(user.getId());
            if(homework == null){
                homework = new Homework();
            }
            model.addAttribute("w", homework);
            model.addAttribute("message", homeworkRepository.findAll());
        }
        catch (Exception e){
            e.printStackTrace();
        }
        return "homework/homework_s";
    }

    @GetMapping("/exercise_s")
    public String exerciseS(Model model) {
        SysUser user = sysUserRepository.findByUsername(SecurityContextHolder.getContext().getAuthentication().getName());
        Exercise exercise = exerciseRepository.findById(user.getId());
        if(exercise == null){
            exercise = new Exercise();
        }
        model.addAttribute("rar", exercise);
        return "homework/exercise_s";
    }

    @GetMapping("/get_exercise")
    public String getExercise() {return "homework/exercise_data"; }

    @PostMapping("/get_rar")
    public String getRar(@RequestParam("file") MultipartFile file) {
        SysUser user = sysUserRepository.findByUsername(SecurityContextHolder.getContext().getAuthentication().getName());
        Exercise exercise = exerciseRepository.findById(user.getId());
        File f = new File("");
        if(exercise != null){
            String path = f.getAbsolutePath() + "\\classes\\static\\homework\\exercise\\" + exercise.getFileName();
            f = new File(path);
            if(f.exists()){
                f.delete();
            }
            exerciseRepository.delete(exercise);
        }
        exercise = new Exercise();
        exercise.setId(user.getId());
        exercise.setFileName(file.getOriginalFilename());
        exercise.setTimeout(System.currentTimeMillis());
        exercise.setGrade("未评分");
        exerciseRepository.save(exercise);
        String filePath = f.getAbsolutePath() + "\\classes\\static\\homework\\";
        try {
            Func.uploadFile(file.getBytes(), filePath, exercise.getFileName());
        }
        catch (Exception e){
            e.printStackTrace();
        }
        return "user/student";
    }

    @PostMapping("/data_delete")
    public String dataDelete(@RequestParam("Id")long Id, Model model){
        Exercise exercise = exerciseRepository.findById(Id);
        File f = new File("");
        f = new File(f.getAbsolutePath() + "\\classes\\static\\homework\\" + exercise.getFileName());
        if(f.exists()){
            f.delete();
        }
        exerciseRepository.delete(exercise);
        List<Exercise>exercises = exerciseRepository.findAll();
        model.addAttribute("message", exercises);
        return "homework/exercise";
    }

    @PostMapping("/data_grade")
    public String dataGrade(@RequestParam("Id")long Id, @RequestParam("Grade")long grade, Model model){
        Exercise exercise = exerciseRepository.findById(Id);
        exercise.setGrade(String.valueOf(grade));
        exerciseRepository.delete(exercise);
        exerciseRepository.save(exercise);
        List<Exercise>exercises = exerciseRepository.findAll();
        model.addAttribute("message", exercises);
        return "homework/exercise";
    }

    @PostMapping("/date_update")
    public String dataUpdate(Model model, @RequestParam("file") MultipartFile file){
        String fileName = "exercise.pdf";
        File f = new File("");
        String filePath = f.getAbsolutePath() + "\\classes\\static\\homework\\";
        try{
            Func.uploadFile(file.getBytes(), filePath, fileName);
        }
        catch (Exception e){
            e.printStackTrace();
        }
        List<Exercise>exercises = exerciseRepository.findAll();
        model.addAttribute("message", exercises);
        return "homework/exercise";
    }

    @GetMapping("/get_homework")
    public String getHomework(Model model){
        File f = new File("");
        String filePath = f.getAbsolutePath() + "\\classes\\static\\homework\\homework.txt";
        try{
            FileInputStream fis = new FileInputStream(filePath);
            InputStreamReader is = new InputStreamReader(fis, "UTF-8");
            BufferedReader br = new BufferedReader(is);
            String title = br.readLine();
            String date = br.readLine();
            String nus = br.readLine();
            model.addAttribute("title", title);
            model.addAttribute("date", date);
            int num = Integer.valueOf(nus);
            String[] problems = new String[num];
            for (int i=0;i<num;i++){
                problems[i] = br.readLine();
            }
            model.addAttribute("problems", problems);
        }
        catch (Exception e){
            e.printStackTrace();
        }
        return "homework/homework_data";
    }

    @PostMapping("/work_delete")
    public String workDelete(@RequestParam("Id")long Id, Model model){
        File f = new File("");
        Homework work = homeworkRepository.findById(Id);
        String path = f.getAbsolutePath() + "\\classes\\static\\homework\\homework\\" + work.getFileName();
        f = new File(path);
        if(f.exists()){
            f.delete();
        }
        homeworkRepository.delete(work);
        List<Homework>homework = homeworkRepository.findAll();
        model.addAttribute("message", homework);
        return "homework/homework";
    }

    @PostMapping("/work_grade")
    public String workGrade(@RequestParam("Id")long Id, @RequestParam("Grade")long grade, Model model){
        Homework work = homeworkRepository.findById(Id);
        homeworkRepository.delete(work);
        work.setGrade(String.valueOf(grade));
        homeworkRepository.save(work);
        List<Homework>homework = homeworkRepository.findAll();
        model.addAttribute("message", homework);
        return "homework/homework";
    }

    @PostMapping("/work_update")
    public String workUpdate(Model model, @RequestParam("file") MultipartFile file){
        File f = new File("");
        String path = f.getAbsolutePath() + "\\classes\\static\\homework\\";
        String name = "homework.txt";
        try {
            Func.uploadFile(file.getBytes(), path, name);
        }
        catch (Exception e){
            e.printStackTrace();
        }
        List<Homework>homework = homeworkRepository.findAll();
        model.addAttribute("message", homework);
        return "homework/homework";
    }

    @PostMapping("/save_work")
    public String saveWork(HomeworkResponse homeworkResponse, Model model){
        StringBuilder builder = new StringBuilder();
        homeworkResponse.change();
        String[] p = homeworkResponse.getP();
        for(int i=0;i<homeworkResponse.count();i++){
            builder.append(p[i]);
            builder.append("\r\n");
        }
        SysUser user = sysUserRepository.findByUsername(SecurityContextHolder.getContext().getAuthentication().getName());
        Homework homework = homeworkRepository.findById(user.getId());
        if(homework == null){
            homework = new Homework();
        }else{
            homeworkRepository.delete(homework);
        }
        homework.setId(user.getId());
        homework.setGrade("未评分");
        homework.setTimeout(System.currentTimeMillis());
        homework.setFileName(user.getUsername()+".txt");
        File f = new File("");
        String path = f.getAbsolutePath() + "\\classes\\static\\homework\\homework\\";
        try{
            Func.uploadFile(builder.toString().getBytes(), path, homework.getFileName());
            homeworkRepository.save(homework);
        }
        catch (Exception e){
            e.printStackTrace();
        }
        model.addAttribute("w", homework);
        return "homework/homework_s";
    }


    @RequestMapping(value = "/he/{Id}")
    public void he(@PathVariable("Id") String Id, HttpServletResponse res){
        File f = new File("");
        String path = f.getAbsolutePath() + "\\classes\\static\\homework\\homework\\";
        Homework work = homeworkRepository.findById(Long.valueOf(Id).longValue());
        res.setHeader("content-type", "application/octet-stream");
        res.setContentType("application/octet-stream");
        res.setHeader("Content-Disposition", "attachment;filename=" + work.getFileName());
        byte[] buff = new byte[1024];
        BufferedInputStream bis = null;
        OutputStream os = null;
        try {
            os = res.getOutputStream();
            bis = new BufferedInputStream(new FileInputStream(new File(path + work.getFileName())));
            int i = bis.read(buff);
            while (i != -1) {
                os.write(buff, 0, buff.length);
                os.flush();
                i = bis.read(buff);
            }
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            if (bis != null) {
                try {
                    bis.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }

}
