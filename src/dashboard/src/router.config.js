import React from 'react';
const Home = React.lazy(() => import('./pages/Home'));
const SignIn = React.lazy(() => import('./pages/SignIn'));
const SignUp = React.lazy(() => import('./pages/SignUp'));
const EmptyTeam = React.lazy(() => import('./pages/EmptyTeam'));
const Submission = React.lazy(() => import('./pages/Submission'));
const JobV2 = React.lazy(() => import('./pages/JobV2'));
const JobsV2 = React.lazy(() => import('./pages/JobsV2'));
const ClusterStatus = React.lazy(() => import('./pages/ClusterStatus'));
const Vc = React.lazy(() => import('./pages/Vc/index.js'));

export default [
  {
    path: '/home',
    component: Home,
    exact: true
  },
  {
    path: '/submission',
    component: Submission,
    needPermission: ['SUBMIT_TRAINING_JOB']
  },
  {
    path: '/jobs-v2/:clusterId/:jobId',
    component: JobV2,
    strict: true,
    exact: true,
    needPermission: ['SUBMIT_TRAINING_JOB', 'VIEW_AND_MANAGE_ALL_USERS_JOB', 'VIEW_ALL_USER_JOB']
  },
  {
    path: '/jobs-v2/:clusterId/',
    component: JobsV2,
    strict: true,
    exact: true,
    needPermission: ['SUBMIT_TRAINING_JOB', 'VIEW_AND_MANAGE_ALL_USERS_JOB', 'VIEW_ALL_USER_JOB']
  },
  {
    path: '/jobs-v2/',
    component: JobsV2,
    strict: true, 
    exact: true,
    needPermission: ['SUBMIT_TRAINING_JOB', 'VIEW_AND_MANAGE_ALL_USERS_JOB', 'VIEW_ALL_USER_JOB']
  },
  {
    path: '/cluster-status',
    component: ClusterStatus,
    needPermission: ['VIEW_CLUSTER_STATUS']
  },
  {
    path: '/vc',
    component: Vc,
    needPermission: ['VIEW_VC', 'MANAGE_VC']
  }
];