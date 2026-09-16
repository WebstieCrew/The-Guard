export const PUBLIC_PATHS = {
  LOGIN: '/login',
  REGISTER: '/register',
  FORGOT_PASSWORD: '/forgot-password',
  UNAUTHORIZED: '/unauthorized',
  NOT_FOUND: '/404',
};

export const PRIVATE_PATHS = {
  DASHBOARD: '/dashboard',
  PROFILE: '/profile',
  SETTINGS: '/settings',
};

export const ADMIN_PATHS = {
  MANAGE_USERS: '/admin/users',
  SYSTEM_LOGS: '/admin/logs',
};

export const PATHS = {
  ...PUBLIC_PATHS,
  ...PRIVATE_PATHS,
  ...ADMIN_PATHS,
};

export const isPublicPath = (path) => {
  return Object.values(PUBLIC_PATHS).includes(path);
};
