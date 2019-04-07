package com.spring.eduspace.functions;

import java.io.File;
import java.io.FileOutputStream;
import java.io.FileWriter;
import java.util.Date;
import java.util.Random;

public class Func {

    private static final int PASSWORDLENGTH = 32;
    private static final int ORIGINLENGTH = 62;
    private static final String ORIGINPASSWORD = "0123456789qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM";

    public static final int JDBCKEY = 1;
    public static final int JSPKEY = 2;
    public static final int SERVLETKEY = 3;
    public static final int SPRINGBOOTKEY = 4;

    @org.jetbrains.annotations.NotNull
    public static String generateRandomPassword(){
        Random rand = new Random();
        Date date = new Date();
        rand.setSeed(date.getTime());
        StringBuilder builder = new StringBuilder();
        for(int i=0;i<PASSWORDLENGTH;i++){
            builder.append(ORIGINPASSWORD.charAt(rand.nextInt(ORIGINLENGTH)));
        }
        return builder.toString();
    }

    public static void uploadFile(byte[] file, String filePath, String fileName) throws Exception {
        File targetFile = new File(filePath);
        if(!targetFile.exists()){
            targetFile.mkdirs();
        }
        FileOutputStream out = new FileOutputStream(filePath+fileName);
        out.write(file);
        out.flush();
        out.close();
    }


}
