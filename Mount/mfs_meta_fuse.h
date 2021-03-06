#ifndef _MFS_META_FUSE_H_
#define _MFS_META_FUSE_H_

#include <fuse_lowlevel.h>

#if FUSE_USE_VERSION >= 26
void mfs_meta_statfs(fuse_req_t req, fuse_ino_t ino);
#else
void mfs_meta_statfs(fuse_req_t req);
#endif

void mfs_meta_lookup(fuse_req_t req, fuse_ino_t parent, const char *name);
void mfs_meta_getattr(fuse_req_t req, fuse_ino_t ino, struct fuse_file_info *fi);
void mfs_meta_setattr(fuse_req_t req, fuse_ino_t ino, struct stat *stbuf, int to_set, struct fuse_file_info *fi);
void mfs_meta_unlink(fuse_req_t req, fuse_ino_t parent, const char *name);
void mfs_meta_rename(fuse_req_t req, fuse_ino_t parent, const char *name, fuse_ino_t newparent, const char *newname);
void mfs_meta_opendir(fuse_req_t req, fuse_ino_t ino, struct fuse_file_info *fi);
void mfs_meta_readdir(fuse_req_t req, fuse_ino_t ino, size_t size, off_t off, struct fuse_file_info *fi);
void mfs_meta_releasedir(fuse_req_t req, fuse_ino_t ino, struct fuse_file_info *fi);
void mfs_meta_open(fuse_req_t req, fuse_ino_t ino, struct fuse_file_info *fi);
void mfs_meta_release(fuse_req_t req, fuse_ino_t ino, struct fuse_file_info *fi);
void mfs_meta_read(fuse_req_t req, fuse_ino_t ino, size_t size, off_t off, struct fuse_file_info *fi);
void mfs_meta_write(fuse_req_t req, fuse_ino_t ino, const char *buf, size_t size, off_t off, struct fuse_file_info *fi);
void mfs_meta_init(int debug_mode_in,double entry_cache_timeout_in,double attr_cache_timeout_in);

#endif
