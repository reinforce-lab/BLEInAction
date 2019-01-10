# Copyright (c) 2006-2018 Minero Aoki, Kenshi Muto, Masayoshi Takahashi, Masanori Kado.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.

require 'fileutils'
require 'rake/clean'

# フォルダをS3に同期して、CloudFrontのキャッシュをクリアする。
task :deploy do
#WEBROOT
  #HTMLの生成タスクを実行する。WEBROOT に出力される。
  Rake::Task["html_all"].execute

  sh "echo #{WEBROOT}/"
  #sh "echo /usr/local/bin/aws s3 sync --acl public-read --delete #{WEBROOT} s3://hoehoge.domain.name.com/ --storage-class STANDARD_IA --region ap-northeast-1 ;aws cloudfront create-invalidation --distribution-id hogehogeIdentifier --paths '/*'"
end
