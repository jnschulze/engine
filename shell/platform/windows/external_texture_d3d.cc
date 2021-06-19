// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "flutter/shell/platform/windows/external_texture_d3d.h"

namespace flutter {

std::unique_ptr<ExternalTextureD3D>
ExternalTextureD3D::MakeFromSurfaceDescriptor(
    const FlutterDesktopGpuSurfaceDescriptor* descriptor,
    AngleSurfaceManager* surface_manager) {
  EGLSurface surface = EGL_NO_SURFACE;
  EGLint attributes[] = {EGL_WIDTH,
                         static_cast<EGLint>(descriptor->width),
                         EGL_HEIGHT,
                         static_cast<EGLint>(descriptor->height),
                         EGL_TEXTURE_TARGET,
                         EGL_TEXTURE_2D,
                         EGL_TEXTURE_FORMAT,
                         EGL_TEXTURE_RGBA,  // always EGL_TEXTURE_RGBA
                         EGL_NONE};

  surface = eglCreatePbufferFromClientBuffer(
      surface_manager->egl_display(), EGL_D3D_TEXTURE_2D_SHARE_HANDLE_ANGLE,
      descriptor->handle, surface_manager->egl_config(), attributes);

  if (surface == EGL_NO_SURFACE) {
    return {};
  }

  return std::unique_ptr<ExternalTextureD3D>(
      new ExternalTextureD3D(surface_manager->egl_display(), surface,
                             descriptor->width, descriptor->height));
}

ExternalTextureD3D::ExternalTextureD3D(EGLDisplay egl_display,
                                       EGLSurface surface,
                                       size_t width,
                                       size_t height)
    : egl_surface_(surface),
      width_(width),
      height_(height),
      egl_display_(egl_display) {}

ExternalTextureD3D::~ExternalTextureD3D() {
  if (egl_surface_) {
    eglDestroySurface(egl_display_, egl_surface_);
  }

  const auto& gl = ResolveGlFunctions();
  if (gl.valid && gl_texture_ != 0) {
    gl.glDeleteTextures(1, &gl_texture_);
  }
}

bool ExternalTextureD3D::PopulateTexture(size_t width,
                                         size_t height,
                                         FlutterOpenGLTexture* opengl_texture) {
  const auto& gl = ResolveGlFunctions();

  if (gl_texture_ == 0) {
    gl.glGenTextures(1, &gl_texture_);

    gl.glBindTexture(GL_TEXTURE_2D, gl_texture_);

    gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (eglBindTexImage(egl_display_, egl_surface_, EGL_BACK_BUFFER) !=
        EGL_TRUE) {
      gl.glDeleteTextures(1, &gl_texture_);
      gl_texture_ = 0;
      return false;
    }
  } else {
    gl.glBindTexture(GL_TEXTURE_2D, gl_texture_);
  }

  opengl_texture->target = GL_TEXTURE_2D;
  opengl_texture->name = gl_texture_;
  opengl_texture->format = GL_RGBA8;
  opengl_texture->destruction_callback = nullptr;
  opengl_texture->user_data = nullptr;
  opengl_texture->width = width_;
  opengl_texture->height = height_;

  return true;
}

}  // namespace flutter
