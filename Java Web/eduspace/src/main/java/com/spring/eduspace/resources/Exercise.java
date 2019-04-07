package com.spring.eduspace.resources;

import javax.persistence.Entity;
import javax.persistence.Id;

@Entity
public class Exercise {
    @Id
    private long id;
    private long timeout;
    private String fileName;
    private String grade;

    public String getFileName() {
        return fileName;
    }

    public long getId() {
        return id;
    }

    public long getTimeout() {
        return timeout;
    }

    public String getGrade() {
        return grade;
    }

    public void setFileName(String fileName) {
        this.fileName = fileName;
    }

    public void setId(long id) {
        this.id = id;
    }

    public void setGrade(String grade) {
        this.grade = grade;
    }

    public void setTimeout(long timeout) {
        this.timeout = timeout;
    }
}
