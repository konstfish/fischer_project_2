# uncomment as necessary
project('bubble', 'cpp',
        license : ['Boost'],
        version : '0.0.5',
        default_options : ['cpp_std=c++17', 'warning_level=3', 'werror=false'])

gnutls_dep = dependency('gnutls')

# begin asio: https://think-async.com/Asio/
#add_global_arguments('-I' + get_option('asio_include_dir'), language : 'cpp')
#add_global_arguments('-DASIO_STANDALONE', language : 'cpp')
#add_global_arguments('-DASIO_NO_DEPRECATED', language : 'cpp')
# end asio

# CLI11: https://github.com/CLIUtils/CLI11
add_global_arguments('-I' + get_option('cli11_include_dir'), language : 'cpp')

# spdlog: https://github.com/gabime/spdlog
add_global_arguments('-I' + get_option('spdlog_include_dir'), language : 'cpp')

# json: https://github.com/nlohmann/json
add_global_arguments('-I' + get_option('json_include_dir'), language : 'cpp')

# tabluate
add_global_arguments('-I' + get_option('tabulate_include_dir'), language : 'cpp')


# # begin protobuf: https://developers.google.com/protocol-buffers/
protoc = find_program('protoc', required : true)
protobuf_dep = dependency('protobuf', required : true)

grpc = find_program('grpc_cpp_plugin', required : true)
grpc_dep = dependency('grpc', required : true)

cppc = meson.get_compiler('cpp')

grpcpp = dependency('grpc++', required: false)
if not grpcpp.found()
  # find the lib without pkg-config
  grpcpp = cppc.find_library('grpc++')
endif

gen = generator(protoc, \
   output    : ['@BASENAME@.pb.cc', '@BASENAME@.pb.h'],
   arguments : ['--proto_path=@CURRENT_SOURCE_DIR@/src', '--cpp_out=@BUILD_DIR@', '@PLAINNAME@'])


grpc_gen = generator(protoc,
  output    : ['@BASENAME@.grpc.pb.cc', '@BASENAME@.grpc.pb.h'],
  arguments : ['--proto_path=@CURRENT_SOURCE_DIR@/src',
    '--grpc_out=@BUILD_DIR@',
    '--plugin=protoc-gen-grpc=' + grpc.path(),
    '@INPUT@'])

generated_pb = gen.process('src/pop3.proto')
generated_rpc = grpc_gen.process('src/pop3.proto')
# # end protobuf

inc_dir = include_directories([ 'include',
                                get_option('cli11_include_dir'),
                                get_option('spdlog_include_dir'),
                                get_option('json_include_dir'),
                                get_option('tabulate_include_dir')                           
                              ])


src = ['src/main.cpp', 'src/POP3client.cpp', 'src/Util.cpp', 'src/Interactive.cpp', 'src/ProtoInterface.cpp', 'src/POP3CSClient.cpp', generated_pb, generated_rpc]

executable('bubble',
           sources : src,
           include_directories : inc_dir,
           dependencies : [gnutls_dep, protobuf_dep, grpc_dep, grpcpp]
          )


# http server

inc_dir_http = include_directories([ 'http_server/include',
                                get_option('cli11_include_dir'),
                                get_option('spdlog_include_dir'),
                                get_option('json_include_dir'),
                                get_option('tabulate_include_dir'),
                                get_option('httplib_include_dir'),
                                get_option('inja_include_dir')
                              ])


src_http = ['http_server/src/main.cpp', 'http_server/src/POP3CSClient.cpp', generated_pb, generated_rpc]

executable('bubble_http',
           sources : src_http,
           include_directories : inc_dir_http,
           dependencies : [protobuf_dep, grpc_dep, grpcpp]
          )