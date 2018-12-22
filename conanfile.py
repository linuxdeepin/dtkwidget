from conans import ConanFile, tools
import os


class DtkwidgetConan(ConanFile):
    name = 'dtkwidget'
    version = '2.0.9'
    license = 'GPL'
    author = 'Iceyer me@iceyer.net'
    url = 'https://github.com/linuxdeepin/dtkwidget'
    description = 'cross platform ui library'
    topics = ('qt', 'dtk')
    settings = 'os', 'compiler', 'build_type', 'arch'
    options = {'shared': [True, False]}
    default_options = 'shared=False'
    generators = 'qmake'
    exports_sources = "*"
    requires = 'qt/5.6.3@iceyer/stable', 'dtkcore/2.0.9@iceyer/stable', 'OpenSSL/1.0.2n@conan/stable', 'jom_installer/1.1.2@bincrafters/stable'

    def extend_include_path(self):
        return '%s/include/libdtk-%s/DWidget' % (self.package_folder, self.version)

    def include_install_path(self):
        return '/include/libdtk-%s' % self.version

    # def source(self):
    #     self.run('git clone https://github.com/linuxdeepin/dtkwidget.git')
    #     self.run('cd dtkwidget && git checkout 2.0.9.9')

    def build(self):
        outdir = self.build_folder
        includedir = outdir + self.include_install_path()
        mkspecsdir = outdir + '/mkspecs'
        libdir = outdir + '/lib'

        env_vars = tools.vcvars_dict(self.settings)
        env_vars['_CL_'] = '/utf-8'
        with tools.environment_append(env_vars):
            command = 'qmake -r'
            command += ' VERSION=%s' % self.version
            # command += ' CONFIG-=debug_and_release'
            # command += ' CONFIG-=debug_and_release_target'
            command += ' CONFIG+=release'
            command += ' PREFIX=%s' % outdir
            command += ' LIB_INSTALL_DIR=%s' % libdir
            command += ' INCLUDE_INSTALL_DIR=%s' % includedir
            command += ' MKSPECS_INSTALL_DIR=%s' % mkspecsdir
            command += ' DTK_STATIC_LIB=YES'
            command += ' DTK_STATIC_TRANSLATION=YES'
            command += ' DTK_NO_MULTIMEDIA=YES'
            command += ' %s' % self.source_folder
            self.run(command)
            self.run('jom clean')
            self.run('jom')
            self.run('jom install')

    def package(self):
        self.deploy()

        outdir = self.build_folder
        self.copy('*', dst='include', src=outdir+'/include')
        self.copy('*.lib', dst='lib', src=outdir+'/lib')
        self.copy('*', dst='mkspecs', src=outdir+'/mkspecs')

    def package_info(self):
        self.cpp_info.libs = ['dtkwidget']
        self.cpp_info.includedirs.append(self.extend_include_path())

        self.env_info.QMAKEPATH = self.cpp_info.rootpath + \
            os.pathsep + self.deps_env_info.QMAKEPATH
        self.env_info.QMAKEFEATURES = self.cpp_info.rootpath + \
            '/mkspecs/features' + os.pathsep+self.deps_env_info.QMAKEFEATURES

    def deploy(self):
        try:
            content = []
            module_pri = self.build_folder + '/mkspecs/modules/qt_lib_dtkwidget.pri'
            s = open(module_pri)
            for line in s.readlines():
                if line.startswith('QT.dtkwidget.tools'):
                    line = 'QT.dtkwidget.tools = %s\n' % (
                        self.package_folder + '/bin')
                elif line.startswith('QT.dtkwidget.libs'):
                    line = 'QT.dtkwidget.libs = %s\n' % (
                        self.package_folder + '/lib')
                elif line.startswith('QT.dtkwidget.includes'):
                    line = 'QT.dtkwidget.includes = %s\n' % (
                        self.extend_include_path())
                content.append(line)
            s.close()

            # print("create module file", content)

            s = open(module_pri, 'w')
            s.writelines(content)
        except FileNotFoundError:
            print("skip update qt module file")
