// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FLUTTER_SHELL_PLATFORM_WINDOWS_EXTERNAL_TEXTURE_D3D_H_
#define FLUTTER_SHELL_PLATFORM_WINDOWS_EXTERNAL_TEXTURE_D3D_H_

#include <stdint.h>

#include <memory>

#include "flutter/shell/platform/common/public/flutter_texture_registrar.h"
#include "flutter/shell/platform/windows/angle_surface_manager.h"
#include "flutter/shell/platform/windows/external_texture.h"

namespace flutter {

// An external texture that is backed by a DXGI surface.
class ExternalTextureD3D : public ExternalTexture {
 public:
  // Creates a texture using the provided FlutterDesktopGpuSurfaceDescriptor.
  static std::unique_ptr<ExternalTextureD3D> MakeFromSurfaceDescriptor(
      const FlutterDesktopGpuSurfaceDescriptor* descriptor,
      AngleSurfaceManager* surface_manager);

  bool PopulateTexture(size_t width,
                       size_t height,
                       FlutterOpenGLTexture* opengl_texture) override;

  virtual ~ExternalTextureD3D();

 private:
  EGLDisplay egl_display_;
  EGLSurface egl_surface_;
  GLuint gl_texture_ = 0;
  size_t width_;
  size_t height_;

  ExternalTextureD3D(EGLDisplay egl_display,
                     EGLSurface egl_surface,
                     size_t width,
                     size_t height);
};

}  // namespace flutter

#endif  // FLUTTER_SHELL_PLATFORM_WINDOWS_EXTERNAL_TEXTURE_D3D_H_
