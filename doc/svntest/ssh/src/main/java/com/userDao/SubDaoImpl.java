package com.userDao;

import com.userEntity.Subnode;
import com.userEntity.User;
import org.hibernate.Query;
import org.hibernate.Session;
import org.hibernate.SessionFactory;
import org.hibernate.Transaction;
import org.springframework.orm.hibernate3.support.HibernateDaoSupport;
import org.springframework.stereotype.Repository;
import org.springframework.transaction.annotation.Transactional;

import javax.annotation.Resource;
import java.io.Serializable;
import java.util.List;

/**
 * Created by churui on 2017/7/29.
 */
@Repository("subDao")
public class SubDaoImpl extends HibernateDaoSupport implements SubDao{
    @Resource(name = "sessionFactory")
    public void setSessionFactoryOverride(SessionFactory sessionFactory)
    {
        super.setSessionFactory(sessionFactory);
    }
    @Override
    public void addsub(Subnode node) {
        Session session = this.getHibernateTemplate().getSessionFactory().openSession();
//        Transaction transaction = session.beginTransaction();
//        transaction.begin();
        Serializable id =session.save(node);
//        transaction.commit();
        session.close();
    }

    @Override
    public List<Subnode> findbynodeid(Integer nodeid) {
        Session session = this.getHibernateTemplate().getSessionFactory().openSession();
//        Transaction transaction = session.beginTransaction();
//        transaction.begin();
        String hql = "from Subnode where nodetbl.nodeid=:nodeid";
        Query query = session.createQuery(hql);
        query.setInteger("nodeid",nodeid);
        List<Subnode> list = query.list();
//        transaction.commit();
        session.close();
        return list;
    }
}
