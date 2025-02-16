#!/usr/bin/env python3
import autograde
import os, os.path

assignment_name = 'PA1'
release='1'

autograde.Test.time_limit = 60

class ErrorTests(autograde.StringTests):
    def get_tests(self, project, prog, build_dir, data_dir):
        test_group = project + ':' + self.name if self.name else project

        test_file = os.path.join(data_dir, self.file)

        if not os.path.exists(test_file):
            logger.warning('Test file not found: %r', test_file)
            return

        autograde.logger.debug('Opening tests file: %r', test_file)

        with open(test_file) as lines:
            try:
                while True:
                    arg = next(lines).rstrip()
                    ref = next(lines).rstrip()

                    yield self.Test(cmd      = self.make_cmd('./' + prog, arg),
                                    ref      = ref,
                                    ref_code = 1,
                                    category = self.category,
                                    group    = test_group,
                                    weight   = self.weight,
                                    dir      = build_dir)

            except StopIteration:
                return


assignment = autograde.MultiProject(
    autograde.StringTests.Project('rot13', weight=0.5),
    autograde.StringTests.Project('palindrome', weight=0.625),
    autograde.Project('balance',
        autograde.StringTests(id='1', name='', weight=0.75),
        ErrorTests(id='2', name='', weight=0.75),
    ),
    autograde.StdinFileTests.Project('list', weight=2),
    autograde.FileTests.Project('mexp', weight=2),
    autograde.Project('bst',
        autograde.StdinFileTests(id='1', weight=1.5),
        autograde.StdinFileTests(id='2', weight=1.5),
    ),
)


if __name__ == '__main__':
    os.environ['LC_ALL'] = 'en-US'
    autograde.main(assignment_name, assignment, release)
