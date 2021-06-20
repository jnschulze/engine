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

typedef struct ExternalTextureD3dState ExternalTextureD3dState;

// An external texture that is backed by a DXGI surface.
class ExternalTextureD3d : public ExternalTexture {
 public:
  ExternalTextureD3d(
      const FlutterDesktopGpuSurfaceTextureCallback texture_callback,
      void* user_data,
      const AngleSurfaceManager* surface_manager,
      const GlProcs& gl_procs);
  virtual ~ExternalTextureD3d();

  bool PopulateTexture(size_t width,
                       size_t height,
                       FlutterOpenGLTexture* opengl_texture) override;

 private:
  bool CreateOrUpdateTexture(
      const FlutterDesktopGpuSurfaceDescriptor* descriptor);

  std::unique_ptr<ExternalTextureD3dState> state_;
  const FlutterDesktopGpuSurfaceTextureCallback texture_callback_ = nullptr;
  void* const user_data_ = nullptr;
  const AngleSurfaceManager* surface_manager_;
  const GlProcs& gl_;
};

}  // namespace flutter

#endif  // FLUTTER_SHELL_PLATFORM_WINDOWS_EXTERNAL_TEXTURE_D3D_H_
