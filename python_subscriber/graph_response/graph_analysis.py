"""This module holds the GraphAnalysis class which is responsible for
generating an impact analysis graph .png image from an Impact Analysis Response
protobuf object.
"""
import os
import matplotlib.pyplot as plt
import matplotlib
import pandas as pd

class GraphAnalysis():
    """Responsible for generating an impact analysis graph .png image
  from an Impact Analysis Response protobuf object.
    """

    def compare_impact(self, queue_impact):
        """Sorts the queue impacts so the impacts where the new SLA is greater then
    the desired impact or increased from the previous SLA.

        Args:
                queue_impact : the list of queue impacts

        Returns:
                str: the desired delta SLA
        """
        desired_delta = (queue_impact.desired_SLA_min - queue_impact.new_SLA_min) + \
    (queue_impact.previous_SLA_min - queue_impact.new_SLA_min)
        return desired_delta


    def graph_impact(self, impact_analysis):
        """Creates the impact analysis .png image and saves it to the project's directory.

        Args:
                impact_analysis (ImpactAnalsisResponse): the ImpactAnalysisResponse Protobuf object

        Returns:
                str: the path to the graph impact analysis .png file
        """
        data = {"Previous SLA" : {}, "New SLA" : {}}
        i = 0
        queue_impact_list = impact_analysis.queue_Impact_analysis_list
        queue_impact_list.sort(key=self.compare_impact)
        matplotlib.use('tkagg')
        # Find the lowest y value for desired/new/prev SLA.
        y_min = queue_impact_list[0].previous_SLA_min

        for queue_impact in queue_impact_list:
            i += 1 # TODO(ballah): i variable used to insure the queue_id property is valid,
        #  and will be cleaned up later.
            queue_id = queue_impact.queue_id or i
            data["Previous SLA"][queue_id] = queue_impact.previous_SLA_min
            data["New SLA"][queue_id] = queue_impact.new_SLA_min
            if queue_impact.previous_SLA_min < y_min:
                y_min = queue_impact.previous_SLA_min
            if queue_impact.new_SLA_min < y_min:
                y_min = queue_impact.new_SLA_min
            if queue_impact.desired_SLA_min < y_min:
                y_min = queue_impact.desired_SLA_min

        data_frame = pd.DataFrame(data)
        a_x = data_frame.plot(kind='bar')

        a_x.set_ylabel('SLA (minutes)')
        a_x.set_xlabel('Queue Id')
        a_x.set_title('Queue Impact Analysis')
        i = 0

        for queue_impact in queue_impact_list:
            i = i + 1
            queue_id = queue_impact.queue_id or i
            # Plot the desired SLA lines and add one label to the legend.
            plt.hlines(y=queue_impact.desired_SLA_min, xmin=i-1.4, xmax=i-.6, label=(i == 1) \
                and "Desired SLA" or None)

        # Get labels for data that has been plotted to ax.
        handles, labels = a_x.get_legend_handles_labels()

        # Plot the legend.
        plt.legend(handles=handles, loc=(1.05, .5))
        plt.tight_layout()
        plt.ylim(ymin=y_min/2)
        plt.xticks(rotation=0)

        path = os.getcwd() + "/impact.png"
        plt.savefig(path)

        return path
