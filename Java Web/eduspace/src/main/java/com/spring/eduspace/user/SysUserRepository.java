package com.spring.eduspace.user;

import org.springframework.data.jpa.repository.JpaRepository;

public interface SysUserRepository extends JpaRepository<SysUser, Long> {
    SysUser findByUsername(String username);
    SysUser findById(long id);
    SysUser findByName(String name);
}