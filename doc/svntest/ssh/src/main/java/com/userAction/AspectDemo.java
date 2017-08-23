package com.userAction;

import org.aspectj.lang.annotation.Aspect;
import org.aspectj.lang.annotation.Before;
import org.springframework.stereotype.Component;

/**
 * Created by churui on 2017/7/20.
 */
@Component
@Aspect
public class AspectDemo {
    @Before("execution(* com.userAction.UserAction.*.*(..))")
    public void testAop(){
        System.out.println("方法前插入AOP");
    }
}
