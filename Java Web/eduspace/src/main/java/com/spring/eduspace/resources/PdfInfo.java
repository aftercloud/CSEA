package com.spring.eduspace.resources;

import javax.persistence.Entity;
import javax.persistence.Id;

@Entity
public class PdfInfo {
    @Id
    private long id;
    private long type;
    private String fileName;
    private String filePath;

    public long getId() {
        return id;
    }

    public long getType() {
        return type;
    }

    public String getFileName() {
        return fileName;
    }

    public String getFilePath() {
        return filePath;
    }

    public void setId(long id) {
        this.id = id;
    }

    public void setType(long type) {
        this.type = type;
    }

    public void setFileName(String fileName) {
        this.fileName = fileName;
    }

    public void setFilePath(String filePath) {
        this.filePath = filePath;
    }
}
